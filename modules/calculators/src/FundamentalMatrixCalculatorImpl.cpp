#include "FundamentalMatrixCalculatorImpl.h"

#include "HomographyCalculatorImpl.h"
#include "LineSegmentIntersectionCalculatorImpl.h"
#include "template_utils.h"

using namespace std;

namespace fvision {

//================== reimplement using template ransac and template dlt =========>
class FundamentalMatrixConstraints : public ConstraintsConstructor<PointPair> {
private:
	CvMat* A;

public:
	FundamentalMatrixConstraints() {
		A = cvCreateMat(1, 9, CV_64FC1);
	}
	~FundamentalMatrixConstraints() {
		cvReleaseMat(&A);
	}

public:
	const CvMat* construct(const PointPair& sample) const {
		double x1 = sample.first.x;
		double y1 = sample.first.y;
		double x2 = sample.second.x;
		double y2 = sample.second.y;

		cvmSet(A, 0, 0, x2 * x1);
		cvmSet(A, 0, 1, x2 * y1);
		cvmSet(A, 0, 2, x2);
		cvmSet(A, 0, 3, y2 * x1);
		cvmSet(A, 0, 4, y2 * y1);
		cvmSet(A, 0, 5, y2);
		cvmSet(A, 0, 6, x1);
		cvmSet(A, 0, 7, y1);
		cvmSet(A, 0, 8, 1);

		return A;
	}
};

CvMatWrapper DltFundamentalMatrixCalculator::compute(const PointPairs& samples) {
	FundamentalMatrixConstraints c;
	GenericDlt<PointPair> dlt;
	CvMat* f = dlt.compute(samples, c);

	CvMat row_header, *F;
	F = cvReshape(f, &row_header, 0, 3);

	//normalize
	CvMat* u = cvCreateMat(F->rows, F->rows, F->type);
	CvMat* vt = cvCreateMat(F->cols, F->cols, F->type);
	CvMat* w = cvCreateMat(F->rows, F->cols, F->type);

	cvSVD(F, w, u, vt, CV_SVD_V_T);

	cvmSet(w, 2, 2, 0);
	CvMat* F2 = matMul(u,w, vt);

	cvReleaseMat(&u);
	cvReleaseMat(&w);
	cvReleaseMat(&vt);
	cvReleaseMat(&f);

	CvMatWrapper fw(F2);

	cvReleaseMat(&F2);

	return fw;
}

CvMatWrapper DltEssentialMatrixCalculator::compute(const PointPairs& samples) {

	FundamentalMatrixConstraints c;
	GenericDlt<PointPair> dlt;
	CvMat* f = dlt.compute(samples, c);

	CvMat row_header, *F;
	F = cvReshape(f, &row_header, 0, 3);

	//normalize
	CvMat* u = cvCreateMat(F->rows, F->rows, F->type);
	CvMat* vt = cvCreateMat(F->cols, F->cols, F->type);
	CvMat* w = cvCreateMat(F->rows, F->cols, F->type);

	cvSVD(F, w, u, vt, CV_SVD_V_T);

	cvmSet(w, 2, 2, 0);

	double m = (cvmGet(w, 0, 0) + cvmGet(w, 1, 1)) / 2;
	cvmSet(w, 0, 0, m);
	cvmSet(w, 1, 1, m);

	CvMat* E = matMul(u, w, vt);

	CvMatWrapper ew(E);

	cvReleaseMat(&u);
	cvReleaseMat(&w);
	cvReleaseMat(&vt);
	cvReleaseMat(&f);
	cvReleaseMat(&E);

	return ew;
}


//F = T2' * nF * T1
CvMat* FundamentalMatrixDenormalizer::denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2) {

	CvMat* T2t = transpose(T2);
	CvMat* F = matMul(T2t, nmodel, T1);
	cvReleaseMat(&T2t);

	return F;
}

IntersectionBasedFundamentalMatrixCalculator::IntersectionBasedFundamentalMatrixCalculator(double ransacThreshold) {
	assert (ransacThreshold >= 0);
	this->ransacThreshold = ransacThreshold;
}

CvMatWrapper IntersectionBasedFundamentalMatrixCalculator::compute(const PointPairs& samples) {

	RANSAC_LineSegmentIntersectionCalculator* ransacE = RANSAC_LineSegmentIntersectionCalculator::create(ransacThreshold);
	NormalizedLineSegmentIntersectionCalculator* nE = new NormalizedLineSegmentIntersectionCalculator(ransacE);
	e = nE->compute(samples);
	//e = ransacE->compute(samples);
	inlierIndices = ransacE->getInlierIndices();
	outlierIndices = ransacE->getOutlierIndices();

	CvMat* ex = getCrossMatrix(e.getMat());

	CvMatWrapper fw = ex;
	cvReleaseMat(&ex);

	cout<<"e: "<<e<<endl;

	return fw;
}

HomographyBasedFundamentalMatrixCalculator::HomographyBasedFundamentalMatrixCalculator(
	double ransacThreshold) 
	: FundamentalMatrixCalculator() 
{
	assert (ransacThreshold >= 0);
	this->ransacThreshold = ransacThreshold;
	setThreshold(ransacThreshold);
	essentialMatrix = false;
}

CvMatWrapper HomographyBasedFundamentalMatrixCalculator::compute(const PointPairs& samples) {

	RANSAC_FundamentalMatrixCalculator* ransacF = RANSAC_FundamentalMatrixCalculator::create(fRansacThreshold);
	ransacF->compute(samples);
	initialFInlierIndices = ransacF->getInlierIndices();
	initialFOutlierIndices = ransacF->getOutlierIndices();
	initialFInliers = ransacF->getInliers();
	initialFOutliers = ransacF->getOutliers();
	initialFInliersRate = ransacF->getInliersRate();

	RANSAC_HomographyCalculator* ransacH = RANSAC_HomographyCalculator::create(hRansacThreshold);
	H = ransacH->compute(initialFInliers);
	hInlierIndices = getSubSet(initialFInlierIndices, ransacH->getInlierIndices());
	hOutlierIndices = getSubSet(initialFInlierIndices, ransacH->getOutlierIndices());
	hInliers = ransacH->getInliers();
	hOutliers = ransacH->getOutliers();
	hInliersRateInF = (double)hInlierIndices.size() / initialFInliers.size();

	PointPairs outliers = hOutliers;
	outliers.insert(outliers.end(), initialFOutliers.begin(), initialFOutliers.end());
	vector<int> outlierIndices = hOutlierIndices;
	outlierIndices.insert(outlierIndices.end(), initialFOutlierIndices.begin(), initialFOutlierIndices.end());

	vector<CvPoint2D32f> ox1s, ox2s;
	decomposePointPairs(outliers, ox1s, ox2s);

	vector<CvPoint2D32f> hox1s = transformPoints(H.getMat(), ox1s);
	PointPairs epps;
	composePointPairs(epps, hox1s, ox2s);

	RANSAC_LineSegmentIntersectionCalculator* ransacE = RANSAC_LineSegmentIntersectionCalculator::create(eRansacThreshold);
	e = ransacE->compute(epps);
	eInlierIndices = getSubSet(outlierIndices, ransacE->getInlierIndices());
	eOutlierIndices = getSubSet(outlierIndices, ransacE->getOutlierIndices());
	eInliers = ransacE->getInliers();
	eOutliers = ransacE->getOutliers();
	eInliersRate = (double)eInlierIndices.size() / epps.size();


	CvMat* ex = getCrossMatrix(e.getMat());
	CvMat* F = matMul(ex, H.getMat());

	CvMatWrapper fw = F;

	//
	FundamentalMatrixGeometricErrorEvaluator geoEval(new FundamentalMatrixGeometricErrorCalculator(), fRansacThreshold);
	FundamentalMatrixEvaluationResults evalResults = geoEval.compute(fw, samples);
	finalFInlierIndices = evalResults.inlierIndices;
	finalFOutlierIndices = evalResults.outlierIndices;

	clearMats(&ex, &F);

	if (essentialMatrix) {
		CvMat* I = getIdentity();
		CvMat* E = getEFromFK(fw.getMat(), I, I);
		CvMatWrapper ew = E;
		clearMats(&I, &E);
		return ew;
	}

	return fw;
}

void HomographyBasedFundamentalMatrixCalculator::setThreshold(double threshold) {
	this->fRansacThreshold = threshold;
	this->hRansacThreshold = threshold * 2.0;
	this->eRansacThreshold = threshold;
}


}
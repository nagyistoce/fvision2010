#include <fvision/cvutils.h>
#include <fvision/moca.h>

#include <utility>
#include <vector>

using namespace fvision;
using namespace std;

typedef std::pair<CvPoint2D32f, CvPoint2D32f> PointPair;
typedef std::vector<PointPair> PointPairs;

typedef GenericModelCalculator<CvMatWrapper, PointPair> LineSegmentIntersectionCalculator;
typedef GenericErrorCaclculator<CvMatWrapper, PointPair> LineSegmentIntersectionErrorCalculator;
typedef ModelEvaluationResults<PointPair> LineSegmentIntersectionEvaluationResults;

class LineSegmentIntersectionConstraints : public ConstraintsConstructor<PointPair> {

	CvMat* A;

public:
	LineSegmentIntersectionConstraints() { A = cvCreateMat(1, 3, CV_64FC1);	}
	~LineSegmentIntersectionConstraints() {	cvReleaseMat(&A); }

	const CvMat* construct(const PointPair& sample) const {
		cvmSet(A, 0, 0, sample.first.y - sample.second.y);
		cvmSet(A, 0, 1, -sample.first.x + sample.second.x);
		cvmSet(A, 0, 2, sample.first.x * sample.second.y - sample.first.y * sample.second.x);
		return A;
	}
};

class DltLineSegmentIntersectionCalculator : public LineSegmentIntersectionCalculator {
public:
	CvMatWrapper compute(const PointPairs& samples){
		LineSegmentIntersectionConstraints c;
		GenericDlt<PointPair> dlt;
		CvMat* h = dlt.compute(samples, c);
		CvMatWrapper hw(h);
		cvReleaseMat(&h);
		return hw;
	}
};

class LineSegmentIntersectionGeometricErrorCalculator : public LineSegmentIntersectionErrorCalculator {
public:
	LineSegmentIntersectionGeometricErrorCalculator(int metric = 1){
		assert (metric == 1 || metric == 2);
		this->metric = metric;
		midPointBuf = cvCreateMat(3, 1, CV_64FC1);
		cvmSet(midPointBuf, 2, 0, 1.0);
		lineBuf = cvCreateMat(3, 1, CV_64FC1);
	}

	~LineSegmentIntersectionGeometricErrorCalculator(){
		cvReleaseMat(&midPointBuf);
		cvReleaseMat(&lineBuf);
	}
public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const{
		cvmSet(midPointBuf, 0, 0, (sample.first.x + sample.second.x) / 2);
		cvmSet(midPointBuf, 1, 0, (sample.first.y + sample.second.y) / 2);
		cvCrossProduct(fw.getMat(), midPointBuf, lineBuf);
		return point2LineDistance(sample.first, lineBuf, metric);
	}
private:
	int metric;
	CvMat* midPointBuf;
	CvMat* lineBuf;
};

class LineSegmentIntersectionAlgebraicErrorCalculator : public LineSegmentIntersectionErrorCalculator {
public:
	LineSegmentIntersectionAlgebraicErrorCalculator(){
		lineBuf = cvCreateMat(3, 1, CV_64FC1);
	}
	~LineSegmentIntersectionAlgebraicErrorCalculator(){
		cvReleaseMat(&lineBuf);
	}
public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const{
		getLine(sample.first, sample.second, lineBuf);
		return abs(cvDotProduct(fw.getMat(), lineBuf));
	}
private:
	CvMat* lineBuf;
};

class RANSAC_LineSegmentIntersectionCalculator : public Ransac<CvMatWrapper, PointPair> {
public:
	RANSAC_LineSegmentIntersectionCalculator(LineSegmentIntersectionCalculator* pmc, int modelSampleSize, LineSegmentIntersectionErrorCalculator* pec) 
        : Ransac<CvMatWrapper, PointPair>(pmc, modelSampleSize, pec) {
	}
};

typedef ModelEvaluator<CvMatWrapper, PointPair, LineSegmentIntersectionGeometricErrorCalculator> LineSegmentIntersectionGeometricErrorEvaluator;
typedef ModelEvaluator<CvMatWrapper, PointPair, LineSegmentIntersectionAlgebraicErrorCalculator> LineSegmentIntersectionAlgebraicErrorEvaluator;


int main() {

	LineSegmentIntersectionErrorCalculator* pec = new LineSegmentIntersectionGeometricErrorCalculator();
	pec->setThreshold(0.3);
	RANSAC_LineSegmentIntersectionCalculator* ransac = new RANSAC_LineSegmentIntersectionCalculator(new DltLineSegmentIntersectionCalculator(), 2, pec);

	PointPairs samples;
	CvMatWrapper model = ransac->compute(samples);

    return 0;
}

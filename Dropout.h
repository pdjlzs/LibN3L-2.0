#ifndef DROPOUT
#define DROPOUT

#include "Eigen/Dense"
#include "MyLib.h"
#include "Node.h"

using namespace Eigen;

class DropNode : Node {
public:
	PNode in;
	Mat mask;
	dtype p;
	
public:
	DropNode(){
		clear();
	}

	inline void clear(){
		Node::clear();
		mask.setOnes();
		p = 1.0;
		in = NULL;
	}

	inline void clearValue(){
		Node::clearValue();
		mask.setOnes();
		in = NULL;
	}

	inline void setDropValue(dtype v){
		p = v;
	}
		
	
public:
	//Be careful that the row is the dim of input vector, and the col is the number of input vectors
	void forward(PNode x, bool bTrain) {
		in = x;

		mask = Mat::Ones(in->val.rows(), in->val.cols());
		if (bTrain){
			std::vector<int> indexes;
			for (int i = 0; i < in->val.rows(); ++i)
				indexes.push_back(i);

			int dropNum = (int)(in->val.rows() * p);

			for (int j = 0; j < in->val.cols(); j++){
				random_shuffle(indexes.begin(), indexes.end());
				for (int i = 0; i < dropNum; i++){
					mask(i, j) = 0.0;
				}
			}
		}
		else{
			mask = mask * p;
		}
		
		val = in->val.array() * mask.array();
	}
	
	
	inline void backward(){
		if (in->loss.size() == 0) {
			in->loss = Mat::Zero(in->val.rows(), in->val.cols());
		}
		in->loss = loss.array() * mask.array();
	}
	
};


#endif

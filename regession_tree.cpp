#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include   <stdlib.h>    
#include   <time.h> 

// tree 0.9 74 => 12.74
using namespace std;
vector< vector< double> > totalData;

vector<vector<double>> testData;

const int Dimension = 385;
double stringToDouble(char *a, int pre, int end);
double varOfVector(vector<double> *data);

void getInput();


struct tree {
	int selectedFeature;
	double valueOfFeature;
	double prediction;
	tree* left;
	tree* right;
	tree() {
		selectedFeature = -1;
		valueOfFeature = -1;
		prediction = -1;
		left = NULL;
		right = NULL;
	}
};

double varOfVector(vector<double> *data);


pair<double,int> split(vector<vector<double>> *cur, int index, double valueOfIndex) {
	vector<double> left, right;
	for (int i = 0; i < (*cur).size(); i++) {
		if ((*cur)[i][index] < valueOfIndex) {
			left.push_back((*cur)[i][Dimension]);
		}
		else {
			right.push_back((*cur)[i][Dimension]);
		}
	}
	return pair<double,int>(varOfVector(&left) + varOfVector(&right),left.size());
}

pair<int,double> chooseBestSplit(vector<vector<double>> *cur) {
	vector<double> y;
	for (int i = 0; i < (*cur).size(); i++) {
		y.push_back((*cur)[i][Dimension]);
	}
	double varOfCur = varOfVector(&y);
	if (varOfCur < 1 || (*cur).size() < 4) {
		double mean = 0;
		for (int i = 0; i < (*cur).size(); i++) {
			mean += (*cur)[i][Dimension];
		}
		return pair<int, double>(-1, mean / (*cur).size());
	}
	int bestIndex = 0;
	double bestV = 9999999999;
	int leftSize = 0;
	double bestIndexValue = 0;
	for (int feature = 1; feature < Dimension; feature++) {

		int bbq = rand() % 100;
		if (bbq < 20) {
			continue;
		}
		set<double> a;
		for (int i = 0; i < (*cur).size(); i++){
			a.insert((*cur)[i][feature]);
		}
		//for (set<double>::iterator i = a.begin();
		//	i != a.end(); i++) {
		//	pair<double,int> tmp  = split(cur, feature, *i);
		//	double v = tmp.first;
		//	if (v < bestV && v > 0) {
		//		bestV = v;
		//		bestIndex = feature;
		//		leftSize = tmp.second;
		//		bestIndexValue = *i;
		//	}		
		//}
		//cout << "over" << endl;

		set<double>::iterator mid = a.begin();
		advance(mid, a.size() / 2);
		pair<double,int> tmp  = split(cur, feature, *mid);
		double v = tmp.first;
		if (v < bestV && v > 0) {
			bestV = v;
			bestIndex = feature;
			leftSize = tmp.second;
			bestIndexValue = *mid;
		}
	}
	double mean = 0;
	for (int i = 0; i < (*cur).size(); i++) {
		mean += (*cur)[i][Dimension];
	}
	mean /= (*cur).size();

	if (bestIndex && leftSize && (*cur).size() != leftSize) {
		return pair<int, double>(bestIndex, bestIndexValue);
	}
	//cout << " subData size is " << (*cur).size() << " mean: " << mean << " v :" <<  varOfCur  << endl;
	return pair<int, double>(-1, mean);
}


void buildTree(tree* &root, vector<vector<double>> *input) {
	pair<int, double> re = chooseBestSplit(input);
	//cout << "best index is " << re.first <<  " " << "value is " << re.second << endl;

	if (re.first == -1) {
		root = new tree();
		root->prediction = re.second;
		return;
	}
	vector<vector<double>> left, right;
	int feature = re.first;
	root = new tree();
	root->selectedFeature = feature;
	root->valueOfFeature = re.second;
	for (int i = 0; i < (*input).size(); i++) {
	/*	int bbq = rand() % 100;
		if (bbq < 10) {
			continue;
		}
	*/	if ((*input)[i][feature] < re.second) {
			left.push_back((*input)[i]);
		}
		else {
			right.push_back((*input)[i]);
		}
	}
	buildTree(root->left, &left);

	buildTree(root->right, &right);
}

double cal_tree(tree* root, vector<double> input) {
	tree* bus = root;
	while (bus->selectedFeature != -1) {
		if (input[bus->selectedFeature] < bus->valueOfFeature)  {
			bus = bus->left;
		}
		else {
			bus = bus->right;
		}
	}
	return bus->prediction;
}

int getTreeDepth(tree* a) {
	if (a == NULL) {
		return 0;
	}
	int l = getTreeDepth(a->left);
	int r = getTreeDepth(a->right);
	return max(l, r) + 1;
}

void getTestInput();

double cal_treeList(vector<tree*> treeList, vector<double> a) {
	double re = 0;
	for (int i = 0; i < treeList.size(); i++) {
		re += cal_tree(treeList[i], a);
	}
	return re / treeList.size();
}


int main() {
	srand(time(NULL));
	getInput();
	vector<tree*> treeList;
	vector<double*> weight;
	for (int i = 0; i < 30000; i++) {
		tree* root = NULL;
		vector<vector<double>> boot, rest;
		for (int j = 0; j < totalData.size(); j++) {
			int k = rand() % 100;
			int d = rand() % 10 + 10;
			if (k < d) {
				rest.push_back(totalData[j]);
			}
			else {
				boot.push_back(totalData[j]);
			}
		}
		buildTree(root, &boot);
		int d = getTreeDepth(root);
		cout << "build finish : "<< i <<" d is " << d << endl;
		double check_err = 0;
		for (int i = 0; i < boot.size(); i++) {
			double y = cal_tree(root, boot[i]);
			check_err += (y - boot[i][Dimension]) * (y - boot[i][Dimension]);
		}
		cout << "check error is " << check_err / boot.size() << endl;

		double err = 0;
		for (int i = 0; i < rest.size(); i++) {
			double y = cal_tree(root, rest[i]);
			err += (y - rest[i][Dimension]) * (y - rest[i][Dimension]);
		}
		cout << "predict err is                           " << err / rest.size() << " of " << rest.size()*1.0 / totalData.size() <<  "  " << treeList.size() << endl;
		if (err / rest.size() < 7) {
			treeList.push_back(root);
		}
		if (treeList.size() > 100) {
			break;
		}

	}
	
	double err = 0;
	for (int i = 0; i < totalData.size(); i++) {
		double y = cal_treeList(treeList, totalData[i]);
		err += (y - totalData[i][Dimension]) * (y - totalData[i][Dimension]);
	}
	cout << "total err is " << err / totalData.size() << endl;

	
	getTestInput();
	ofstream upload;
	upload.open("upload.csv");
	upload << "Id,reference" << endl;
	
	
	for (int line = 0; line < testData.size(); line++) {
		double h_y = cal_treeList(treeList, testData[line]);
		upload << testData[line][0] << "," << h_y << endl;
	}
	upload.close();


	return 0;
}




double stringToDouble(char *a, int pre, int end) {
	double re = 0;
	if (a[pre] == '-') {
		int t = 0;
		double small = 0;
		bool isSmall = false;
		double SmallLen = 0.1;
		for (int i = pre + 1; i < end; i++) {
			if (a[i] == '.') {
				isSmall = true;
				continue;
			}
			if (!isSmall) {
				t = t * 10 + a[i] - '0';
			}
			else {
				small += (a[i] - '0') * SmallLen;
				SmallLen /= 10;
			}
		}
		if (isSmall) {
			return -(t + small);
		}
		else{
			return -t;
		}
	}
	else {
		int t = 0;
		double small = 0;
		bool isSmall = false;
		double SmallLen = 0.1;
		for (int i = pre; i < end; i++) {
			if (a[i] == '.') {
				isSmall = true;
				continue;
			}
			if (!isSmall) {
				t = t * 10 + a[i] - '0';
			}
			else {
				small += (a[i] - '0') * SmallLen;
				SmallLen /= 10;
			}
		}
		if (isSmall) {
			return t + small;
		}
		else {
			return t;
		}
	}
}


void getInput() {
	ifstream input;
	input.open("train_temp.csv");
	char tempString[400 * 20];
	// ignore the first line;
	input >> tempString;
	double f = 0;
	while (input >> tempString) {
		if (strlen(tempString) == 0) {
			break;
		}
		vector<double> oneLine;
		int iter = 0;
		int pre = 0;
		while (tempString[iter] != '\0') {
			if (tempString[iter] == ',') {
				oneLine.push_back(stringToDouble(tempString, pre, iter));
				pre = iter + 1;
			}
			iter++;
		}
		oneLine.push_back(stringToDouble(tempString, pre, iter));
		totalData.push_back(oneLine);
	}
	input.close();
	cout << "read data complete" << endl;
}

double varOfVector(vector<double> *data) {
	if ((*data).size() == 0) {
		return 0;
	}
	double m = 0;
	for (int i = 0; i < (*data).size(); i++) {
		m += (*data)[i];
	}
	m /= (*data).size();
	double v = 0;
	for (int i = 0; i < (*data).size(); i++) {
		v += ((*data)[i] - m) *((*data)[i] - m);
	}
	return v;
}
void getTestInput() {
	ifstream input;
	input.open("test_temp.csv");
	char tempString[400 * 20];
	// ignore the first line;
	input >> tempString;
	double f = 0;
	while (input >> tempString) {
		if (strlen(tempString) == 0) {
			break;
		}
		vector<double> oneLine;
		int iter = 0;
		int pre = 0;
		while (tempString[iter] != '\0') {
			if (tempString[iter] == ',') {
				oneLine.push_back(stringToDouble(tempString, pre, iter));
				pre = iter + 1;
			}
			iter++;
		}
		oneLine.push_back(stringToDouble(tempString, pre, iter));
		testData.push_back(oneLine);
	}
	input.close();
	cout << "read test data complete" << endl;
}

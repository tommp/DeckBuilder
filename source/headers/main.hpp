#ifndef MAIN_HPP
#define MAIN_HPP

/*Macro definitions*/
/*---------------------------------------------*/
#define cimg_use_png
#define cimg_use_jpeg
/*---------------------------------------------*/

/*Included headers*/
/*---------------------------------------------*/
#include "CImg.h"
#include <glob.h>
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <sstream>
/*---------------------------------------------*/

using namespace cimg_library;
using std::vector;
using std::string;
using std::ostringstream;

/*Header content*/
/*=============================================*/
namespace main_consts {
	string folder_name = "../data/*";
	string back_path = "../back_smile.jpg";
	string clover_path = "../clover.jpg";
	string sheet_folder = "../sheets/";
	const int clovers_per_row = 10;
	const int cards_per_page = 16;
	const int shortside = 200;
	const int longside = shortside * sqrt(2);
	const int cards_per_row = sqrt(cards_per_page);
	const int cloversize = (shortside*cards_per_row) / clovers_per_row;
}

vector<string> glob(const string& pat){
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<string> ret;
    for(unsigned int i=0; i<glob_result.gl_pathc; i++){
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

void make_backside() {
	CImg<double> backside(main_consts::back_path.c_str());
	backside.resize(main_consts::shortside, main_consts::longside, -100, -100, 5);

	CImg<double> sheet(main_consts::shortside * main_consts::cards_per_row, 
						main_consts::longside * main_consts::cards_per_row,
						1,
						3);

	for (int x = 0; x < main_consts::cards_per_row; x++) {
		for (int y = 0; y < main_consts::cards_per_row; y++){
			for (int back_x = 0;  back_x < backside.width(); back_x++){
				for (int back_y = 0;  back_y < backside.height(); back_y++){
					sheet(x * main_consts::shortside + back_x, y * main_consts::longside + back_y, 0, 0) = backside(back_x,back_y,0,0);
					sheet(x * main_consts::shortside + back_x, y * main_consts::longside + back_y, 0, 1) = backside(back_x,back_y,0,1);
					sheet(x * main_consts::shortside + back_x, y * main_consts::longside + back_y, 0, 2) = backside(back_x,back_y,0,2);
				}
			}
		}
	}

	ostringstream sstream;
	sstream << main_consts::sheet_folder << "backside.jpg";
	string filename = sstream.str();
	sheet.save(filename.c_str());
}

void make_clovers() {

	CImg<double> clover(main_consts::clover_path.c_str());
	clover.resize(main_consts::cloversize, main_consts::cloversize, -100, -100, 5);

	CImg<double> sheet(main_consts::shortside * main_consts::cards_per_row, 
						main_consts::longside * main_consts::cards_per_row,
						1,
						3);

	for (int x = 0; x < main_consts::clovers_per_row; x++) {
		for (int y = 0; y < floor(main_consts::clovers_per_row*sqrt(2)); y++){
			for (int back_x = 0;  back_x < clover.width(); back_x++){
				for (int back_y = 0;  back_y < clover.height(); back_y++){
					sheet(x * main_consts::cloversize + back_x, y * main_consts::cloversize + back_y, 0, 0) = clover(back_x,back_y,0,0);
					sheet(x * main_consts::cloversize + back_x, y * main_consts::cloversize + back_y, 0, 1) = clover(back_x,back_y,0,1);
					sheet(x * main_consts::cloversize + back_x, y * main_consts::cloversize + back_y, 0, 2) = clover(back_x,back_y,0,2);
				}
			}
		}
	}
	ostringstream sstream;
	sstream << main_consts::sheet_folder << "clovers.jpg";
	string filename = sstream.str();
	sheet.save(filename.c_str());
	sheet.fill(0);
}


void make_sheets() {
	vector<string> filenames = glob(main_consts::folder_name);

	int row_index = 0;
	int col_index = 0;

	int sheet_number = 0;

	CImg<double> sheet(main_consts::shortside * main_consts::cards_per_row, 
						main_consts::longside * main_consts::cards_per_row,
						1,
						3);

	for (unsigned int cardnum = 0; cardnum < filenames.size(); cardnum++) {

		CImg<double> card(filenames[cardnum].c_str());
		card.resize(main_consts::shortside, main_consts::longside, -100, -100, 5);

		for (int x = 0; x < card.width(); x++) {
			for (int y = 0; y < card.height(); y++) {
				sheet(row_index * main_consts::shortside + x, col_index * main_consts::longside + y, 0, 0) = card(x,y,0,0);
				sheet(row_index * main_consts::shortside + x, col_index * main_consts::longside + y, 0, 1) = card(x,y,0,1);
				sheet(row_index * main_consts::shortside + x, col_index * main_consts::longside + y, 0, 2) = card(x,y,0,2);
			}
		}
		row_index++;

		if (row_index % main_consts::cards_per_row == 0 && row_index != 0) {
			row_index = 0;
			col_index++;
		}
		if (col_index % main_consts::cards_per_row == 0 && col_index != 0) {
			col_index = 0;

			ostringstream sstream;
			sstream << main_consts::sheet_folder << sheet_number << ".jpg";
			string filename = sstream.str();
			sheet.save(filename.c_str());
			sheet_number++;
			sheet.fill(0);
		}
	}

	if (row_index % main_consts::cards_per_row || col_index % main_consts::cards_per_row) {
		ostringstream sstream;
		sstream << main_consts::sheet_folder << sheet_number << ".jpg";
		string filename = sstream.str();
		sheet.save(filename.c_str());
		sheet_number++;
		sheet.fill(0);
	}
}
/*=============================================*/

#endif
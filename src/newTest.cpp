#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include <sstream>
#include<vector>
#include"include/MMesh.h"
#include"include/Mesh.h"
using namespace MHW;
std::vector<MHW::MyPoint>  Mesh::Get_limitP(std::string path){

    std::cout<<"OKOKOK"<<std::endl;
    std::ifstream in;
    std::string filename=path;
    std::vector<MHW::MyPoint> P_arr;
    in.open(filename);
    if(!in){
        std::cout<<"读取文件错误"<<std::endl;
    }

    std::string V_index,X_c,Y_c,Z_c;
    while(in>>V_index){
        MHW::MyPoint tem;
        // std::cout<<"索引:"<<V_index<<"__";
        in>>X_c;

        // std::cout<<"X坐标:"<<X_c<<"__";
        in>>Y_c;
        // std::cout<<"Y坐标:"<<Y_c<<"__";
        in>>Z_c;
        //std::cout<<"Z坐标:"<<Z_c<<std::endl;
        tem.index=atoi(V_index.c_str());
        tem.X=atof(X_c.c_str());
        tem.Y=atof(Y_c.c_str());
        tem.Z=atof(Z_c.c_str());
        P_arr.push_back(tem);
    }
    in.close();
    return P_arr;
}

std::vector<MHW::MyPoint>  Mesh::Get_limitP_fM(QVector<QVector<int> > Select_P_Array){
    std::vector<MHW::MyPoint> ret;
    for(int i=0;i<Select_P_Array.size();i++){
        for(int j=0;j<Select_P_Array[i].size();j++){
            MHW::MyPoint tempV_P;
            OpenMesh::VertexHandle tempV(Select_P_Array[i][j]);
            auto iter=this->point(tempV);
            tempV_P.index=tempV.idx();
            tempV_P.X=iter[0];
            tempV_P.Y=iter[1];
            tempV_P.Z=iter[2];
            ret.push_back(tempV_P);

        }
    }
    return ret;
}

bool Mesh::is_limitP(std::vector<MyPoint> LMT_point,int indexN){

    for(int i=0;i<LMT_point.size();i++){
        if(LMT_point[i].index==indexN){
            return true;
        }

    }
    return false;
}

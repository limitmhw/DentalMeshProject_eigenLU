#ifndef MESH_H
#define MESH_H

//
//  BoundingBox.h
//  Reconstructor
//
//  Created by sway on 6/13/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "include/BoundingBox.h"


namespace SW{

class MPoint{
public:
    double x;
    double y;
    double z;
    MPoint(double x,double y):x(x),y(y),z(0){}
    MPoint(double x,double y,double z):x(x),y(y),z(z){}
    MPoint(const MPoint& p){
        x=p.x;
        y=p.y;
        z=p.z;
    }

};
struct MVector{
public:
    QVector<double> X_arr;
    QVector<double>Y_arr;
    QVector<double>Z_arr;

};

struct Halfedge_angle{
    double edge_length;
    double f_angle_halftan;
    double t_angle_halftan;
};
struct Angle_point{
    OpenMesh::VertexHandle Point;
    double halftan;
};

struct MyPoint{
    int index;
    double X;
    double Y;
    double Z;
};
//*********************************************************
class Mesh:public OpenMesh::TriMesh_ArrayKernelT<>
{
public:
    Mesh(QString name);
    Mesh(QString name, int NPoint);
    Mesh();

    void getBoundingBox();

    // 0--vertices 1-- wireframe 2-- flatLine
    void draw(int flag,QVector< QVector<int> > Select_P_Array, MVector MoveVectors);
    bool isSelectP(VertexHandle vh, QVector<QVector<int> > Select_P_Array, int *Belong_PS);

    //****************************************************
    //Transform form test
    //****************************************************
    bool readModel(std::string Mod_Path);
    bool writeModel(std::string Write_path);
    double Gethalfedge_length(OpenMesh::HalfedgeHandle hh);
    OpenMesh::VertexHandle Getopposite_point(OpenMesh::HalfedgeHandle hh);
    double GetHtan_angleV(double Close_edgeA_length,double Close_edgeB_length,double Opposite_edge_length);
    void HandleFace_EA(OpenMesh::FaceHandle);
    std::vector<MyPoint> Get_limitP(std::string path);
    std::vector<MyPoint>Get_limitP_fM(QVector<QVector<int> > Select_P_Array,MVector MoveVectors);
    bool is_limitP(std::vector<MyPoint> LMT_point,int indexN);

    std::vector<Halfedge_angle> Hedge_angle;


   // MHW::LTransform TryL;
    //****************************************************
public:
    BoundingBox BBox;
    QString MeshName;


};
}
#endif // MESH_H

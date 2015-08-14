#ifndef UNION_H
#define UNION_H

#endif // UNION_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <iostream>
#include <fstream>
#include"include/Mesh.h"


#include <CGAL/basic.h>
#include <CGAL/Inverse_index.h>
#include<OpenMesh/Core/Mesh/AttribKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Polyhedron_3<CGAL::Simple_cartesian<double> > Polyhedron_d;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Aff_transformation_3 Aff_transformation_3;
typedef Polyhedron::HalfedgeDS         HalfedgeDS;


typedef typename Polyhedron::Vertex_const_iterator                  VCI;
typedef typename Polyhedron::Facet_const_iterator                   FCI;
typedef typename Polyhedron::Halfedge_around_facet_const_circulator HFCC;
using namespace SW;



struct CGALtoOpenMesh{
    int Openindex;
    Mesh::Point pp;
};
template <class HDS>
class Build_triangle : public CGAL::Modifier_base<HDS> {
public:
    Mesh MyMesh;
    Build_triangle() {}
    void operator()( HDS& hds) {
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( MyMesh.n_vertices(), MyMesh.n_faces(), 0);//面起始
    //    std::cout<<MyMesh.n_vertices()<<std::endl;
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        std::vector<int> ChangeIndex;
        int GP_js=0;
        ChangeIndex.resize(MyMesh.n_vertices(),4);//设置容器大小
        for(int i=0;i<MyMesh.n_vertices();i++){
            ChangeIndex[i]=-1;//初始化
        }
        for(auto it=MyMesh.faces_begin();it!=MyMesh.faces_end();++it){    //遍历openmesh所有面
            int j=0;
            int FP[3];
            for(auto it2=MyMesh.fv_begin(it.handle());it2!=MyMesh.fv_end(it.handle());++it2){
                auto point=MyMesh.point(it2.handle());
           //     std::cout<<"--------x"<<point[0]<<"--------y"<<point[1]<<"---------z"<<point[2]<<std::endl;
                FP[j]=it2.handle().idx();//保存索引号
                if(ChangeIndex[FP[j]]==-1){
                    B.add_vertex( Point( point[0], point[1], point[2]));
                }
                j++;
            }
            B.begin_facet();
            for(int i=0;i<3;i++){
                if(ChangeIndex[FP[i]]==-1){

                    ChangeIndex[FP[i]]=GP_js;
                    GP_js++;
                    B.add_vertex_to_facet(ChangeIndex[FP[i]]);

                }else{
                    B.add_vertex_to_facet(ChangeIndex[FP[i]]);
                }
            }
            B.end_facet();
        }
        B.end_surface();
    }
};
class union_mod{
public:
    Polyhedron P1;
    Polyhedron P2;
    Polyhedron P12;
    Build_triangle<HalfedgeDS> triangle1,triangle2;
    //get data
    union_mod(SW::Mesh M1,SW::Mesh M2){
        triangle1.MyMesh=M1;
        triangle2.MyMesh=M2;
        P1.delegate(triangle1);
        P2.delegate(triangle2);
    }
    Mesh UnionOutMod(){
        UnionOut();
        std::vector<CGALtoOpenMesh> CtoO;
        Mesh NewMesh_of_CGAL;

        for( FCI fi = P12.facets_begin(); fi != P12.facets_end(); ++fi){
            std::vector<OpenMesh::VertexHandle> FacePoint;
            int i=0;
            for( HFCC hc = fi->facet_begin();i<3;++hc,i++){

                double x_point=CGAL::to_double(hc->vertex()->point().x());
                double y_point=CGAL::to_double(hc->vertex()->point().y());
                double z_point=CGAL::to_double(hc->vertex()->point().z());

                if(CheckEixstP(CtoO,x_point,y_point,z_point)!=-1){//旧点
                    OpenMesh::VertexHandle tt(CheckEixstP(CtoO,x_point,y_point,z_point));
                    FacePoint.push_back(tt);
                }else{//新点
                    CGALtoOpenMesh temp;

                    temp.pp[0]=x_point;
                    temp.pp[1]=y_point;
                    temp.pp[2]=z_point;
                    OpenMesh::VertexHandle ss_tem=NewMesh_of_CGAL.add_vertex(temp.pp);
                    temp.Openindex=ss_tem.idx();
                    CtoO.push_back(temp);
                    FacePoint.push_back(ss_tem);

                }
            }

            NewMesh_of_CGAL.add_face(FacePoint[0],FacePoint[1],FacePoint[2]);
        }
        return NewMesh_of_CGAL;
    }

private:
    void UnionOut(){
        Nef_polyhedron N1(P1);
        Nef_polyhedron N2(P2);
        Nef_polyhedron N12;
        Aff_transformation_3 aff(CGAL::TRANSLATION, Vector_3(0.1,0.11,0,1));
        N2.transform(aff);
        //  N1+=N2;  // model-dependent crash here
        N12 = N1.join(N2);
        N12.closure().convert_to_polyhedron(P12);

    }

    int CheckEixstP( std::vector<CGALtoOpenMesh> & CtoO,double xx,double yy,double zz){

        for(int i=0;i<CtoO.size();i++){
            if(CtoO[i].pp[0]==xx){
                if(CtoO[i].pp[1]==yy){
                    if(CtoO[i].pp[2]==zz){
                        return CtoO[i].Openindex;
                    }
                }
            }

        }

        return -1;
    }

};

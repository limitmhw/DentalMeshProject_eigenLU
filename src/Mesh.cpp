#include"include/Mesh.h"
#include<QDebug>

using namespace SW;


Mesh::Mesh(){}
//****************************//
//MHW_Change
//****************************//
Mesh::Mesh(QString name):MeshName(name){}
Mesh::Mesh(QString name,int NPoint):MeshName(name){}

void Mesh::getBoundingBox(){}

// 0--vertices 1-- wireframe 2-- flatLine
void Mesh::draw(int flag,   QVector< QVector<int> > Select_P_Array,MVector MoveVectors){

    switch (flag){
    case 0:
        glPointSize(10);
        glPushMatrix();
        //  glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        for(Mesh::HalfedgeIter hit= this->halfedges_begin(); hit!= this->halfedges_end(); hit++){
            Mesh::VertexHandle vh = to_vertex_handle(hit);
            Mesh::Point v = this->point(vh);

            //normal
            Mesh::Normal nor;
            calc_vertex_normal_loop(vh, nor);
            glNormal3f(nor[0], nor[1], nor[2]);
            //position
            //*************************************
            //MHW;ADD
            //*************************************
            int Belong_PS;
            if(isSelectP(vh,Select_P_Array,&Belong_PS)){
                glColor3f(0.0f, 0.0f, 1.0f);

            }else{
                glColor3f(1.0f, 0.0f, 0.0f);

            }
            //*************************************
            glVertex3f(v[0], v[1], v[2]);
        }
        glEnd();
        glPopMatrix();
        break;
    case 1:
        glPushMatrix();
        // glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        for(Mesh::EdgeIter eit = edges_begin(); eit!= edges_end(); eit++){
            Mesh::HalfedgeHandle hh1 = halfedge_handle(eit, 0);
            Mesh::HalfedgeHandle hh2 = halfedge_handle(eit, 1);
            Mesh::VertexHandle vh1 = to_vertex_handle(hh1);
            Mesh::Point v1 = this->point(vh1);

            Mesh::VertexHandle vh2 = to_vertex_handle(hh2);
            Mesh::Point v2 = this->point(vh2);
            //*************************************
            //MHW;ADD
            //*************************************
            int Belong_PS1;
            if(isSelectP(vh1,Select_P_Array,&Belong_PS1)){
                glColor3f(0.0f, 0.0f, 1.0f);


            }else{

                glColor3f(1.0f, 0.0f, 0.0f);


            }
            //*************************************
            glVertex3f(v1[0], v1[1], v1[2]);
            //*************************************
            //MHW;ADD
            //*************************************
            int Belong_PS2;
            if(isSelectP(vh2,Select_P_Array,&Belong_PS2)){
                glColor3f(0.0f, 0.0f, 1.0f);

            }else{
                glColor3f(1.0f, 0.0f, 0.0f);

            }
            //*************************************
            glVertex3f(v2[0], v2[1], v2[2]);
        }
        glEnd();
        glPopMatrix();
        break;
    case 2:
        // glColor3f(0.0f, 0.0f, 1.0f);
        //request_vertex_normals();
        for(Mesh::FaceIter fit = faces_begin(); fit!= faces_end(); fit++){

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_POLYGON);

            Mesh::Normal nor = calc_face_normal(fit);
            glNormal3f(nor[0], nor[1], nor[2]);
            for(Mesh::FaceHalfedgeIter fhit = fh_iter(*fit);
                fhit.is_valid(); ++fhit){




                Mesh::VertexHandle vh = to_vertex_handle(fhit);
                Mesh::Point v = this->point(vh);
                //*************************************
                //MHW;ADD
                //*************************************
                int Belong_PS=90;
                if(isSelectP(vh,Select_P_Array,&Belong_PS)){
                    glColor3f(0.0f, 0.0f, 1.0f);
                    if(Belong_PS==90){
                        qDebug() << "moveJH:"<<Belong_PS<< endl;

                    }

                    glVertex3f(v[0]+(0.001*MoveVectors.X_arr[Belong_PS]), v[1]+ (0.001*MoveVectors.Y_arr[Belong_PS]), v[2]);
                    //glVertex3f(v[0], v[1], v[2]);
                }else{
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glVertex3f(v[0], v[1], v[2]);
                }
                //*************************************
                //glVertex3f(v[0], v[1], v[2]);
            }
            glEnd();
        }

        break;

    }

}
bool Mesh::isSelectP(Mesh::VertexHandle vh,QVector<QVector<int> > Select_P_Array,int* Belong_PS){
    for(int i=0;i<Select_P_Array.size();i++){
        for(int j=0;j<Select_P_Array[i].size();j++){

            if(vh.idx()==Select_P_Array[i][j]){
                *(Belong_PS)=i;
                return true;

            }
        }
    }
    return false;
}

//****************************************************************************************
//Transform form test
//****************************************************************************************
//****************************************************************************************

bool Mesh::writeModel(std::string Write_path){
    bool ret=OpenMesh::IO::write_mesh(*(this),Write_path);
    if(ret==1){
        return 0;// no error;
    }else{
        return 1;// there have an error;
    }
}

bool Mesh::readModel(std::string Mod_Path){

    bool ret=OpenMesh::IO::read_mesh(*(this),Mod_Path);


    this->Hedge_angle.resize(this->n_halfedges());
    //*****************************
    //2015-06-23 TYPE=Notes
    //*****************************
    //我想吐槽一下为什么返回是一个整数;为什么返回1表示没有错误？？？？;
    //*****************************

    if(ret==1){
        return 0;// no error;
    }else{
        return 1;// there have an error;
    }

}

double Mesh::Gethalfedge_length(OpenMesh::HalfedgeHandle hh){

    auto point1=this->point(this->from_vertex_handle(hh));
    auto point2=this->point(this->to_vertex_handle(hh));
    //        std::cout<<"x"<<point1[0]<<"____y"<<point1[1]<<"____z"<<point1[2]<<std::endl;
    //        std::cout<<"x"<<point2[0]<<"____y"<<point2[1]<<"____z"<<point2[2]<<std::endl;
    auto l_temX=(point1[0]-point2[0])*(point1[0]-point2[0]);
    auto l_temY=(point1[1]-point2[1])*(point1[1]-point2[1]);
    auto l_temZ=(point1[2]-point2[2])*(point1[2]-point2[2]);
    return std::sqrt(l_temX+l_temY+l_temZ);

}

OpenMesh::VertexHandle Mesh::Getopposite_point(OpenMesh::HalfedgeHandle hh){


    //    for(auto it=vv.begin();it!=vv.end();++it){

    //        if(*it!=this->from_vertex_handle(hh)&&*it!=this->to_vertex_handle(hh)){
    //           return *it;
    //    }

}

double Mesh::GetHtan_angleV(double Close_edgeA_length,double Close_edgeB_length,double Opposite_edge_length){
    double CosV=((Close_edgeA_length*Close_edgeA_length)+(Close_edgeB_length*Close_edgeB_length)-(Opposite_edge_length*Opposite_edge_length))/(2*Close_edgeA_length*Close_edgeB_length);
    return std::sqrt((1-CosV)/(1+CosV));
}

void Mesh::HandleFace_EA(OpenMesh::FaceHandle face){

    std::vector<OpenMesh::VertexHandle> vv;
    std::vector<OpenMesh::HalfedgeHandle> hh;

    //*****************************
    //2015-06-25 TYPE=Note
    //*****************************
    //取面的点与边;
    //*****************************
    for(auto it=this->fh_begin(face);it!=this->fh_end(face);++it){
        hh.push_back(it.handle());
    }
    for(auto it=this->fv_begin(face);it!=this->fv_end(face);++it){
        vv.push_back(it.handle());
    }
    ////*************Bedug****************
    //    for(int i=0;i<3;i++){

    //        std::cout<<"vv["<<i<<"]:"<<this->point(vv[i])<<std::endl;
    //    }

    //    for(int i=0;i<3;i++){
    //        std::cout<<"hh["<<i<<"]:"<<hh[i]<<std::endl;
    //    }
    ////*************Bedug****************
    //*****************************
    //2015-06-25 TYPE=dd
    //*****************************
    //求各个边的长度;
    //*****************************
    double h0_length=this->Gethalfedge_length(hh[0]);
    double h1_length=this->Gethalfedge_length(hh[1]);
    double h2_length=this->Gethalfedge_length(hh[2]);
    if((h0_length*h1_length*h2_length)==0){
        std::cout<<"Face_edge_length_err"<<face.idx()<<std::endl;
    }


    //*****************************
    //2015-06-25 TYPE=dd
    //*****************************
    //处理边角关系，建立边角关系向量;
    //*****************************

    for(auto it=vv.begin();it!=vv.end();++it){

        if(*it!=this->from_vertex_handle(hh[0])&&*it!=this->to_vertex_handle(hh[0])){

            double Vhalftan=this->GetHtan_angleV(h1_length,h2_length,h0_length);

            if(*it!=this->from_vertex_handle(hh[1])){
                //std::cout<<"不是H1的from角"<<std::endl;
                this->Hedge_angle[hh[1].idx()].edge_length=h1_length;
                this->Hedge_angle[hh[1].idx()].t_angle_halftan=Vhalftan;


                this->Hedge_angle[hh[2].idx()].edge_length=h2_length;
                this->Hedge_angle[hh[2].idx()].f_angle_halftan=Vhalftan;



            }else{
                // std::cout<<"是H1的from角"<<std::endl;
                this->Hedge_angle[hh[1].idx()].edge_length=h1_length;
                this->Hedge_angle[hh[1].idx()].f_angle_halftan=Vhalftan;

                this->Hedge_angle[hh[2].idx()].edge_length=h2_length;
                this->Hedge_angle[hh[2].idx()].t_angle_halftan=Vhalftan;
            }

        }

    }

    for(auto it=vv.begin();it!=vv.end();++it){

        if(*it!=this->from_vertex_handle(hh[1])&&*it!=this->to_vertex_handle(hh[1])){

            double Vhalftan=this->GetHtan_angleV(h0_length,h2_length,h1_length);

            if(*it!=this->from_vertex_handle(hh[0])){
                // std::cout<<"不是H0的from角"<<std::endl;
                this->Hedge_angle[hh[0].idx()].edge_length=h1_length;
                this->Hedge_angle[hh[0].idx()].t_angle_halftan=Vhalftan;


                this->Hedge_angle[hh[2].idx()].edge_length=h2_length;
                this->Hedge_angle[hh[2].idx()].f_angle_halftan=Vhalftan;



            }else{
                // std::cout<<"是H0的from角"<<std::endl;
                this->Hedge_angle[hh[0].idx()].edge_length=h1_length;
                this->Hedge_angle[hh[0].idx()].f_angle_halftan=Vhalftan;

                this->Hedge_angle[hh[2].idx()].edge_length=h2_length;
                this->Hedge_angle[hh[2].idx()].t_angle_halftan=Vhalftan;
            }

        }

    }

    for(auto it=vv.begin();it!=vv.end();++it){

        if(*it!=this->from_vertex_handle(hh[2])&&*it!=this->to_vertex_handle(hh[2])){

            double Vhalftan=this->GetHtan_angleV(h0_length,h1_length,h2_length);

            if(*it!=this->from_vertex_handle(hh[0])){
                // std::cout<<"不是H0的from角"<<std::endl;
                this->Hedge_angle[hh[0].idx()].edge_length=h1_length;
                this->Hedge_angle[hh[0].idx()].t_angle_halftan=Vhalftan;


                this->Hedge_angle[hh[1].idx()].edge_length=h2_length;
                this->Hedge_angle[hh[1].idx()].f_angle_halftan=Vhalftan;



            }else{
                // std::cout<<"是H0的from角"<<std::endl;
                this->Hedge_angle[hh[0].idx()].edge_length=h1_length;
                this->Hedge_angle[hh[0].idx()].f_angle_halftan=Vhalftan;

                this->Hedge_angle[hh[1].idx()].edge_length=h2_length;
                this->Hedge_angle[hh[1].idx()].t_angle_halftan=Vhalftan;
            }

        }

    }


}



std::vector<MyPoint>  Mesh::Get_limitP_fM(QVector<QVector<int> > Select_P_Array, MVector MoveVectors){
    std::vector<MyPoint> ret;
    bool CongFu=false;
    for(int i=0;i<Select_P_Array.size();i++){

        for(int j=0;j<Select_P_Array[i].size();j++){

                for(int k=0;k<ret.size();k++){
                    if(Select_P_Array[i][j]==ret[k].index){
                        CongFu=true;
                    }
                }
                if(!CongFu){
                    MyPoint tempV_P;
                    OpenMesh::VertexHandle tempV(Select_P_Array[i][j]);
                    auto iter=this->point(tempV);
                    tempV_P.index=tempV.idx();
                    // double ctl_l=0.05;
                    double ctl_l=1;
                    tempV_P.X=iter[0]+(ctl_l*MoveVectors.X_arr[i]);
                    tempV_P.Y=iter[1]+(ctl_l*MoveVectors.Y_arr[i]);
                    tempV_P.Z=iter[2]+(ctl_l*MoveVectors.Z_arr[i]);
                    //tempV_P.Z=iter[2];
                    ret.push_back(tempV_P);
                }else{
                    CongFu=false;
                }

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




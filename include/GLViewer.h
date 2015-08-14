#ifndef SW_GLVIEWER_H
#define SW_GLVIEWER_H

//
//  BoundingBox.h
//  Reconstructor
//
//  Created by sway on 6/13/15.
//  Copyright (c) 2015 None. All rights reserved.
//


#include"include/Mesh.h"
#include"QGLViewer/qglviewer.h"




//#include <QtCore/QVariant>
//#include <QtGui/QAction>
//#include <QtGui/QApplication>
//#include <QtGui/QButtonGroup>
//#include <QtGui/QHBoxLayout>
//#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
//#include <QtGui/QMenu>
//#include <QtGui/QMenuBar>
//#include <QtGui/QStatusBar>
//#include <QtGui/QToolBar>
//#include <QtGui/QWidget>
//#include <include/GLViewer.h>



#include<list>
#include<vector>
#include<QObject>
#include<QMouseEvent>
#include<QMenu>
#include<QAction>
# include <QMenu>
#include <qcursor.h>
#include <qmap.h>
#include <math.h>
#include<QColor>
#include<QListWidgetItem>
#include <QImage>
#include<QKeyEvent>

#include<QVector2D>
#include<QVector3D>
#include<QThread>

#include"include/Shader.h"
#include "include/LaplaceTransform.h"

namespace SW{



class  GLViewer : public QGLViewer
{
    Q_OBJECT

public:
    enum DispalyType{VERTICES, WIREFRAME, FLATLINE};
    // GLViewer(QWidget *parent0=0, const QGLWidget *parent1=0, Qt::WFlags f = 0);
    GLViewer(QWidget *parent0=0, const char *parent1=0, QGLWidget *f = 0);
    ~GLViewer();

    virtual void init();
    virtual void draw();
    virtual QString helpString();
    virtual void transform(){}

    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

    virtual void keyPressEvent(QKeyEvent *e);

    void viewAll();

    void addMesh(const Mesh & mesh){
        meshes.append(mesh);
        viewAll();
        Do_L.startL(meshes);
    }
    Mesh getMesh(int i){
        if(i>=0&&i<meshes.size()){
            return meshes[i];
        }
        return Mesh();
    }

    int n_mesh(){
        return meshes.size();
    }
    //为了解决每次重新构建项目时都需要注释ui_mainwindow.h中的两行的问题，添加下面两个空方法
    void setFrameShape(QFrame::Shape){}
    void setFrameShadow(QFrame::Shadow){}

protected:
    //draw world coordinates
    void drawAxises(double width, double length);
    void setGL(void);
    void setLighting(void);
    void initGLSL();
    void setMeshMaterial();

    void drawSelectingWindow();

public slots:
    virtual void drawText(){}

    void toggleDisplayVertices(){
        displayType = VERTICES;
        updateGL();
    }

    void toggleDisplayWireFrame(){
        displayType = WIREFRAME;
        updateGL();
    }

    void toggleDisplayFlatLine(){
        displayType = FLATLINE;
        updateGL();
    }

    void toggleSelectingVertexMode(){
        select_vertices_mode = !select_vertices_mode;
        DrawRect=false;
        if(select_vertices_mode==true){
            setMouseTracking(true);
        }else{
            setMouseTracking(false);
        }
        updateGL();
    }
    void toggleMovePoints(){
        MovePoints_mode=!MovePoints_mode;
        //qDebug() << "toggleMovePoints" << endl;
    }


    void  toggleRestore(){
        meshes[0]=Do_L.P->objMesh;
        for(int i=0;i<MoveVectors.X_arr.size();i++){
            MoveVectors.X_arr[i]=0;
            MoveVectors.Y_arr[i]=0;
            MoveVectors.Z_arr[i]=0;
        }
        Select_P_Array.empty();
        Select_P_Array.clear();
        updateGL();
    }
    void toggleClearModel(){
        meshes.clear();
    }

signals:

public:
     QVector<Mesh> meshes;
private:
    static SW::Shader m_shader;
    float m_length;


    bool displayVertices;
    bool displayWireFrame;
    bool displayFlatLine;

    bool select_vertices_mode;
    bool MovePoints_mode;
    bool display_slecting_rect;

    DispalyType displayType;
    //****************************//
    //MHW_Change
    //****************************//
    MHW::Do_LTransform Do_L;
    bool DrawRect;
    bool P_OnMoving;
    bool OnLaplacian;
    MPoint CurMouse_pos;
    MPoint PressMouse_pos;
    MPoint ReleaseMouse_pos;
    //MPoint MoveVector;
    MVector MoveVectors;
    //QMouseEvent PressPoint;
    //    double PressPoint_x;
    //    double PressPoint_y;
    QRect selecting_window;
    QVector< QVector<int> > Select_P_Array;
    int Cur_choose_P;//当前选择的
    void handleSelectPoint(int meshesNum);
    QVector<QRect> Get2D_SP_Rect(int meshesNum);
    bool IsSelectPoint(int meshesNum,int x,int y,int *S_array_Num);
    void drawMoveLine();
    //****************************//

};
//class  MRect{
//    public:
//        double MaxX,MaxY,MinX,MinY;
//        bool contains(double x,double y);
//};

}
#endif // SW_GLVIEWER_H

#include"include/MainWindow.h"

#include<qfiledialog.h>
#include<QListWidget>
#include<QTextStream>
#include<QGridLayout>
#include<QProgressDialog>
#include<QDebug>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
SW::MainWindow::MainWindow()
{
    setupUi(this);

    connect(actionOpen, SIGNAL(triggered()), this, SLOT(doActionOpen()) );
    connect(actionSave, SIGNAL(triggered()), this, SLOT(doActionSave()) );


    connect(actionLaplacianDeformation, SIGNAL(triggered()), this, SLOT(doActionLaplacianDeformation()) );
    connect(actionUnion, SIGNAL(triggered()), this, SLOT(doActionUnion()));
    connect(actionDisplayVertices, SIGNAL(triggered()), gv, SLOT(toggleDisplayVertices()));
    connect(actionDisplayWireFrame, SIGNAL(triggered()), gv, SLOT(toggleDisplayWireFrame()));
    connect(actionDisplayFlatLine, SIGNAL(triggered()), gv, SLOT(toggleDisplayFlatLine()));
    connect(actionSegmentation, SIGNAL(triggered()), this, SLOT(doActionSegmentation()));

    connect(actionSelectPoints, SIGNAL(triggered()), gv, SLOT(toggleSelectingVertexMode()));
    connect(actionMovePoints, SIGNAL(triggered()), gv, SLOT(toggleMovePoints()));
    connect(actionRestore, SIGNAL(triggered()), gv, SLOT(toggleRestore()));
 connect(actionClearModel, SIGNAL(triggered()), gv, SLOT(toggleClearModel()));

    update();
}
///////////////////////////////////////////////////////////////////////////////////
SW::MainWindow::~MainWindow()
{

}


void SW::MainWindow::doActionOpen(){

    QString fileFilters= "Mesh(*.obj *.off *.ply)";
    QStringList  selectedFiles = QFileDialog::getOpenFileNames(this, tr("Open File(s)"),"/home/sway/",fileFilters);
    if(selectedFiles.size()==0) return;

    foreach(QString filePath, selectedFiles){
        statusBar()->showMessage("Loading Mesh from "+filePath);

        Mesh mesh;
        if(!OpenMesh::IO::read_mesh(mesh, filePath.toStdString().c_str())){
            QMessageBox::information(this,"Error","Error to load "+ filePath);
        }
       int vNum =0;
       int fNum = 0;
       int eNum = 0;
       for(Mesh::VertexIter vit = mesh.vertices_begin(); vit!= mesh.vertices_end(); vit++) vNum++;
       for(Mesh::FaceIter fit = mesh.faces_begin(); fit!= mesh.faces_end(); fit++)fNum++;
       for(Mesh::EdgeIter eit = mesh.edges_begin(); eit!= mesh.edges_end(); eit++)eNum++;

       // If the file did not provide vertex normals, then calculate them
      if ( mesh.has_face_normals() && mesh.has_vertex_normals() ) {
          // let the mesh update the normals
          mesh.update_normals();
      }
       QMessageBox::information(this,"Info",QString("Vertices: %1 \n Facets: %2 \n Edges: %2").arg(vNum).arg(fNum).arg(eNum));
       statusBar()->showMessage("Load Successed!");

       gv->addMesh(mesh);
       //gv->viewAll();
    }

}

void SW::MainWindow::doActionSave(){

    QString fileFilters= "Mesh(*.obj *.off *.ply)";
    QString  selectedFiles = QFileDialog::getSaveFileName(this, tr("Save File"),"/home/limit/MHWMHW.obj",tr("Images (*.png *.xpm *.jpg)"));
           // QFileDialog::getOpenFileNames()

    gv->meshes[0].writeModel(selectedFiles.toStdString());


}


void SW::MainWindow::doActionLaplacianDeformation(){
    /*TODO*/
    //OnLaplacian=!OnLaplacian;
    if(gv->n_mesh()>0){
         SW::Mesh mesh = gv->getMesh(0);
         QMessageBox::information(this,"Info",QString("Vertices: %1 \n Facets: %2 \n Edges: %2").arg(mesh.n_vertices()).arg(mesh.n_faces()).arg(mesh.n_edges()));
    }
}

void SW::MainWindow::doActionUnion(){
    union_mod doUnion(gv->meshes[0],gv->meshes[1]);
    Mesh temmesh=doUnion.UnionOutMod();


    gv->meshes.clear();
    gv->addMesh(temmesh);
    /*TODO*/
}

void SW::MainWindow::doActionSegmentation(){
    if(gv->n_mesh()>0){
         SW::Mesh mesh = gv->getMesh(0);
         QMessageBox::information(this,"Info",QString("Vertices: %1 \n Facets: %2 \n Edges: %2").arg(mesh.n_vertices()).arg(mesh.n_faces()).arg(mesh.n_edges()));
    }
    /*TODO*/


}

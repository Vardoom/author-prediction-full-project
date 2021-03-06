#include <QCoreApplication>

#include <iostream>
#include <sstream>
#include <bits/stdc++.h>

#include "author_classification/Preprocessing/text.h"
#include <author_classification/Preprocessing/buildtxt.cpp>

#include "author_classification_fs/pca.h"

#include "ml_on_author_prediction/RS/multiclass_rs.h"

#include "ml_on_author_prediction/KNN/knn.h"
#include "ml_on_author_prediction/ml_on_author_prediction/Hugo_KNN.h"
#include "ml_on_author_prediction/KNN/myknn_algo.h"

//#include <QtWidgets/QApplication>
//#include <QtWidgets/QMainWindow>
//#include <QtCharts/QChartView>
//#include <QtCharts/QPieSeries>
//#include <QtCharts/QPieSlice>
#include <time.h>

using namespace std;


//QT_CHARTS_USE_NAMESPACE

//void Ensemble(double ** pred,int nombreAlgo,int NombreAuteur,int k,int* AuteurK,double* ProbaK){

//    double res[NombreAuteur];

//    for(int i=0;i<NombreAuteur;i++)
//        res[i]=0;

//    for(int i=0;i<NombreAuteur;i++){
//        for(int j=0;j<nombreAlgo;j++){

//            res[i]=res[i]+pred[j][i];
//        }
//    }

//    for(int i=0;i<NombreAuteur;i++)
//        res[i]=res[i]/nombreAlgo;

//    vector <int> a;


//    for(int i=0;i<NombreAuteur;i++){
//        a.push_back(res[i]);
//    }

//   std::sort(a.begin(), a.end());

//   int b[NombreAuteur];

//    for (int i = 0; i < k; ++i){
//        ProbaK[i]=a.at(k-1-i);

//        for(int j=0;j<NombreAuteur;j++){
//            if(res[j]==ProbaK[i]){
//                b[i]=j;
//                res[j]=-1;
//            }
//        }

//        AuteurK[i]=b[i];
//   }
// }

int main()
{
    time_t timer;
    double new_time;
    new_time = time(&timer);

    int no_authors = 10;
    int lineCount = 0; //nbre de ligne du fichier csv qui résultera du texte test
    float parameter = 0.98;
    std::string c = "../apprentissage_10.txt";
    std::string test_text = "../testResult.txt";
    int numberOfTrees = 1;
    string train = "../finalMatrix.csv";
    string test = "../finalTest.csv";
    int NumbersofNeighbors=3;

    std::cout << "Fetching features..." << std::endl;

    lineCount = preprocessingTest("../1013.txt","txt", "testResult");

    new_time = difftime(time(&timer),new_time);
    std::cout << new_time << " seconds since previous stage" << endl;

    std::cout << "Starting PCA..." << std::endl;
    //principalComponentAnalysis(c,test_text, parameter);

    new_time = difftime(time(&timer),new_time);
    std::cout << new_time << " seconds since previous stage" << endl;

    std::cout << "Learning task..." << endl;

    //machine learning
    std::cout << "Random Forest:" << std::endl;
    multiclass_rs * classifier1 = new multiclass_rs(numberOfTrees, no_authors);
    int * predictions_random_forest = classifier1->run_random_forest(train, test);

    new_time = difftime(time(&timer),new_time);
    std::cout << new_time << " seconds since previous stage" << endl;

//    std::cout << "First KNN:" << endl;
    int * predictions_first_knn = KNN_(NumbersofNeighbors, test, train);
    new_time = difftime(time(&timer),new_time);

    std::cout << new_time << " secondes since previous stage" << endl;
    std::cout << "Turning predictions into probability..." << std::endl;

    double proba_rs [no_authors];
    double proba_first_knn [no_authors];
    for (int i = 0; i < no_authors; i++){
        proba_rs[i] = 0;
        proba_first_knn[i] = 0;
    }

    for (int i = 0; i < no_authors; i++) {
        for (int j = 0; j < lineCount; j++){
            if (predictions_random_forest[j] == i)
                proba_rs[i] += 1;
            if (predictions_first_knn[j] == i)
                proba_first_knn[i] += 1;
        }
    }

    for (int i = 0; i < no_authors; i++){
        proba_rs[i] /= lineCount;
        proba_first_knn[i] /= lineCount;
    }

    new_time = difftime(time(&timer),new_time);
    std::cout << new_time << " seconds since previous stage" << endl;

    for (int i=0; i < no_authors; i++){
        std::cout << IdToAuthor(i) << " : " << proba_rs[i] << endl;
    }

    //hugo
//    std::cout << "Second KNN:" << endl;
//    int no_main_authors = 3;
//    double res[no_main_authors];
//    ExecutionML(train, test, no_main_authors, res);

//    std::cout << "resultat hugo" << res[0] << std::endl;

//    new_time = difftime(time(&timer),new_time);
//    new_time /= 60;
//    std::cout << new_time << " minutes since previous stage" << endl;

//    std::cout << "Combining results of different learners..." << std::endl;

//    int no_algos = 2;
//    double * res2 [no_algos];
//    res2[0] = proba_rs;
//    res2[1] = proba_first_knn;
//    int no_main_authors = 3;
//    double ProbaK[no_main_authors];
//    int AuthorK[no_main_authors];
//    Ensemble(res2, no_algos, no_authors, no_main_authors, AuthorK, ProbaK);

//    for (int i = 0; i < no_main_authors;i++) {
//        cout << "Author " << IdToAuthor(AuthorK[i]) << ":" << ProbaK[i] << endl;
//    }

//    std::cout<< "Preparing pie chart" << std::endl;

//    QApplication a();
//    QPieSeries *series = new QPieSeries();
//    for (int i = 0; i < no_main_authors ; i++) {
//        series->append((char*)AuthorK[i], (int) 100*ProbaK[i]);
//    }
//    QPieSlice *slice = series->slices().at(1);
//    slice->setExploded();
//    slice->setLabelVisible();
//    slice->setPen(QPen(Qt::darkGreen, 2));
//    slice->setBrush(Qt::green);
//    QChart *chart = new QChart();
//    chart->addSeries(series);
//    chart->setTitle("Author?");
//    //chart->legend()->hide();
//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    QMainWindow window;
//    window.setCentralWidget(chartView);
//    window.resize(400, 300);
//    window.show();

//    new_time = difftime(time(&timer),new_time);
//    std::cout << new_time << " seconds since previous stage" << endl;

    return 0;
}

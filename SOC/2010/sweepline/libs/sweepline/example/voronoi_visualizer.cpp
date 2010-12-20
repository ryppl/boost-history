// Boost sweepline library voronoi_visualizer.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <QtOpenGL/QGLWidget>
#include <QtGui/QtGui>

#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

class GLWidget : public QGLWidget {
    Q_OBJECT
public:
    GLWidget(QMainWindow *parent = NULL) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
        startTimer(40);
    }

    QSize sizeHint() const {
        return QSize(600, 600);
    }

    void build(QString file_path) {
        std::vector<ipoint_2d_type> point_sites;
        std::vector<isegment_2d_type> segment_sites;

        // Open file.
        QFile data(file_path);
        if (!data.open(QFile::ReadOnly))
            QMessageBox::warning(this, tr("Voronoi Visualizer"),
                                 tr("Disable to open file ") + file_path);

        // Read points from the file.
        QTextStream in_stream(&data);
        int num_point_sites = 0;
        int num_edge_sites = 0;
        int x1, y1, x2, y2;
        in_stream >> num_point_sites;
        for (int i = 0; i < num_point_sites; i++) {
            in_stream >> x1 >> y1;
            point_sites.push_back(make_point_2d(x1, y1));
        }
        in_stream >> num_edge_sites;
        for (int i = 0; i < num_edge_sites; i++) {
            in_stream >> x1 >> y1 >> x2 >> y2;
            segment_sites.push_back(std::make_pair(
                make_point_2d(x1, y1), make_point_2d(x2, y2)));
        }
        in_stream.flush();

        // Build voronoi diagram.
        build_voronoi(point_sites, segment_sites, voronoi_output_);
        brect_ = voronoi_helper<coordinate_type>::get_view_rectangle(
            voronoi_output_.bounding_rectangle());

        // Update view.
        update_view_port();
    }

protected:
    void paintGL() {
        qglClearColor(QColor::fromRgb(0, 0, 0));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw voronoi sites.
        {
            const voronoi_cells_type &cells = voronoi_output_.cell_records();
            voronoi_cell_const_iterator_type it;
            glColor3f(0.0f, 0.0f, 1.0f);
            glPointSize(9);
            glBegin(GL_POINTS);
            for (it = cells.begin(); it != cells.end(); it++) {
                if (!it->contains_segment())
                    glVertex2f(it->point0().x(), it->point0().y());
            }
            glEnd();
            glPointSize(6);
            glLineWidth(2.7f);
            glBegin(GL_LINES);
            for (it = cells.begin(); it != cells.end(); it++) {
                if (it->contains_segment()) {
                    glVertex2f(it->point0().x(), it->point0().y());
                    glVertex2f(it->point1().x(), it->point1().y());
                }
            }
            glEnd();
            glLineWidth(1.0);
        }

        // Draw voronoi vertices.
        {
            const voronoi_vertices_type &vertices = voronoi_output_.vertex_records();
            voronoi_vertex_const_iterator_type it;
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_POINTS);
            for (it = vertices.begin(); it != vertices.end(); it++)
                glVertex2f(it->vertex().x(), it->vertex().y());
            glEnd();
        }

        // Draw voronoi edges.
        {
            const voronoi_edges_type &edges = voronoi_output_.edge_records();
            voronoi_edge_const_iterator_type it;
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINES);
            for (it = edges.begin(); it != edges.end(); it++) {
                std::vector<point_2d_type> temp_v =
                    voronoi_helper<coordinate_type>::get_intermediate_points(&(*it), brect_);
                for (int i = 0; i < static_cast<int>(temp_v.size()) - 1; i++) {
                    glVertex2f(temp_v[i].x(), temp_v[i].y());
                    glVertex2f(temp_v[i+1].x(), temp_v[i+1].y());
                }
            }
            glEnd();
        }
    }

    void resizeGL(int width, int height) {
        int side = qMin(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);
    }

    void timerEvent(QTimerEvent *) {
        update();
    }

private:
    void update_view_port() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(brect_.x_min(), brect_.x_max(), brect_.y_min(), brect_.y_max(), -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    }

    typedef double coordinate_type;
    typedef point_2d<coordinate_type> point_2d_type;
    typedef point_2d<int> ipoint_2d_type;
    typedef std::pair<ipoint_2d_type, ipoint_2d_type> isegment_2d_type;
    typedef voronoi_output<coordinate_type>::voronoi_cells_type voronoi_cells_type;
    typedef voronoi_output<coordinate_type>::voronoi_vertices_type voronoi_vertices_type;
    typedef voronoi_output<coordinate_type>::voronoi_edges_type voronoi_edges_type;
    typedef voronoi_output<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;
    typedef voronoi_output<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;
    typedef voronoi_output<coordinate_type>::voronoi_edge_const_iterator_type
        voronoi_edge_const_iterator_type;
    BRect<coordinate_type> brect_;
    voronoi_output<coordinate_type> voronoi_output_;
};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow() {
        glWidget_ = new GLWidget();
        file_dir_ = QDir(QDir::currentPath(), tr("*.txt"));
        file_name_ = tr("");

        QHBoxLayout *centralLayout = new QHBoxLayout;
        centralLayout->addWidget(glWidget_);
        centralLayout->addLayout(create_file_layout());
        setLayout(centralLayout);

        update_file_list();
        setWindowTitle(tr("Voronoi Visualizer"));
        layout()->setSizeConstraint( QLayout::SetFixedSize );
    }

private slots:
    void browse() {
        QString new_path = QFileDialog::getExistingDirectory(0, tr("Choose Directory"),
                                                             file_dir_.absolutePath());
        if (new_path.isEmpty())
            return;
        file_dir_.setPath(new_path);
        update_file_list();
    }

    void print_scr() {
        if (!file_name_.isEmpty()) {
            QImage screenshot = glWidget_->grabFrameBuffer(true);
            QString output_file = file_dir_.absolutePath() + tr("/") +
                file_name_.left(file_name_.indexOf('.')) + tr(".png");
            screenshot.save(output_file, 0, -1);
        }
    }

    void build() {
        file_name_ = file_list_->currentItem()->text();
        QString file_path = file_dir_.filePath(file_name_);
        message_label_->setText("Building...");
        glWidget_->build(file_path);
        message_label_->setText("Double click the item to build voronoi diagram:");
        setWindowTitle(tr("Voronoi Visualizer - ") + file_path);
    }

private:
    QGridLayout *create_file_layout() {
        QGridLayout *file_layout = new QGridLayout;

        message_label_ = new QLabel("Double click item to build voronoi diagram:");

        file_list_ = new QListWidget();
        file_list_->connect(file_list_, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                            this, SLOT(build()));

        QPushButton *browse_button = new QPushButton(tr("Browse Input Directory"));
        connect(browse_button, SIGNAL(clicked()), this, SLOT(browse()));
        browse_button->setMinimumHeight(50);

        QPushButton *print_scr_button = new QPushButton(tr("Make Screenshot"));
        connect(print_scr_button, SIGNAL(clicked()), this, SLOT(print_scr()));
        print_scr_button->setMinimumHeight(50);

        file_layout->addWidget(message_label_, 0, 0);
        file_layout->addWidget(file_list_, 1, 0);
        file_layout->addWidget(browse_button, 2, 0);
        file_layout->addWidget(print_scr_button, 3, 0);

        return file_layout;
    }

    void update_file_list() {
        QFileInfoList list = file_dir_.entryInfoList();
        file_list_->clear();
        if (file_dir_.count() == 0)
            return;

        QFileInfoList::const_iterator it;
        for (it = list.begin(); it != list.end(); it++)
            file_list_->addItem(it->fileName());
        file_list_->setCurrentRow(0);
    }

    QDir file_dir_;
    QString file_name_;
    GLWidget *glWidget_;
    QListWidget *file_list_;
    QLabel *message_label_;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

#include "voronoi_visualizer.moc"

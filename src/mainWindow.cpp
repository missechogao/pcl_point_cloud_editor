///
/// Copyright (c) 2012, Texas A&M University
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions
/// are met:
///
///  * Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
///  * Redistributions in binary form must reproduce the above
///    copyright notice, this list of conditions and the following
///    disclaimer in the documentation and/or other materials provided
///    with the distribution.
///  * Neither the name of Texas A&M University nor the names of its
///    contributors may be used to endorse or promote products derived
///    from this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
/// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
/// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
/// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
/// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
///
/// The following software was written as part of a collaboration with the
/// University of South Carolina, Interdisciplinary Mathematics Institute.
///

/// @file mainWindow.cpp
/// @details the implementation of the MainWindow class
/// @author Yue Li and Matthew Hielsberg

#include <algorithm>
#include <pcl/apps/point_cloud_editor/mainWindow.h>
#include <pcl/apps/point_cloud_editor/cloudEditorWidget.h>
#include <pcl/apps/point_cloud_editor/localTypes.h>
#include <QLayout>

MainWindow::MainWindow () :
    window_width_(WINDOW_WIDTH), window_height_(WINDOW_HEIGHT)
{
    initWindow();
}

MainWindow::MainWindow (int argc, char **argv) :
    window_width_(WINDOW_WIDTH), window_height_(WINDOW_HEIGHT)
{
    initWindow();
    if (argc > 1)
        cloud_editor_widget_->loadFile(argv[1]);
}

MainWindow::~MainWindow()
{

}

void
MainWindow::about ()
{
    QMessageBox::about(this, QString("点云编辑器"),
                       tr("PCL 3D Editor\n\nAuthors: \n  Matthew Hielsberg (hielsber@tamu.edu) and\n"
                          "  Yue Li (yli@cse.tamu.edu)\n  Texas A&M University\n\n"
                          "This software was written as part of a collaboration with the "
                          "University of South Carolina, Interdisciplinary Mathematics Institute."));
}

void
MainWindow::help ()
{
    QMessageBox::about(this, tr("点云编辑器"),
                       tr("视图模式\n"
                          "  鼠标拖拽:\t关于坐标原点旋转\n"
                          "  Alt+鼠标拖拽:\t平移Z\n"
                          "  Ctrl+鼠标拖拽:\t缓慢缩放\n"
                          "  Shift+鼠标拖拽:\t缩放\n"
                          "\n"
                          "选择变换模式\n"
                          "  鼠标拖拽:\t关于中心点旋转\n"
                          "  Alt+鼠标拖拽:\t平移Z\n"
                          "  Ctrl+鼠标拖拽:\t缓慢缩放\n"
                          "\n"
                          "鼠标点选模式\n"
                          "  左键:\t\t选择点\n"
                          "  Ctrl+左键 :\t取消选择\n"
                          "  Shift+左键:\t附加选择\n"
                          "\n"
                          "2D选择模式 (Rubberband)\n"
                          "  拖拽鼠标:\t\t选择选区\n"
                          "  选择选区:\t\t取消选区\n"
                          "  Shift+鼠标拖拽:\t附加选择\n"
                          "\n"
                          "快捷键\n"
                          "  1:\t\t变白\n"
                          "  2:\t\t使用色环 X\n"
                          "  3:\t\t使用色环 Y\n"
                          "  4:\t\t使用色环 Z\n"
                          "  5:\t\t使用RGB颜色\n"
                          "  Ctrl C:\t\t复制\n"
                          "  Ctrl X:\t\t剪切\n"
                          "  Ctrl V:\t\t粘贴\n"
                          "  Ctrl Z:\t\t撤销\n"
                          "  V:\t\t视图模式\n"
                          "  T:\t\t选择变换模式\n"
                          "  E:\t\t选择模式\n"
                          "  S:\t\t2D选择模式\n"
                          "  Del:\t\t删除选区\n"
                          "  +:\t\t放大所有点\n"
                          "  -:\t\t缩小所有点\n"
                          "  Ctrl +:\t放大所选点\n"
                          "  Ctrl -:\t缩小所选点\n"
                          "  Esc:\t\t取消选择\n"
                          ));
}

void
MainWindow::initWindow ()
{
    cloud_editor_widget_ = new CloudEditorWidget(this);
    cloud_editor_widget_->setAttribute(Qt::WA_DeleteOnClose);
    setCentralWidget(cloud_editor_widget_);
    createActions();
    createMenus();
    createToolBars();
    setWindowTitle(tr("PCL 3D 编辑器"));
    resize(window_width_, window_height_);
}

void
MainWindow::createActions ()
{
    action_group_ = new QActionGroup(this);

    QString icon_path(":/");

    open_action_ = new QAction(QIcon(icon_path+"open.png"),QString("打开..."),this);
    connect(open_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(load()));

    save_action_ = new QAction(QIcon(icon_path+"save.png"), QString("另存为.."),
                               this);
    connect(save_action_,SIGNAL(triggered()),cloud_editor_widget_,SLOT(save()));

    exit_action_ = new QAction(QString("退出..."), this);
    connect(exit_action_, SIGNAL(triggered()), this, SLOT(close()));

    about_action_ = new QAction(QString("关于"), this);
    connect(about_action_, SIGNAL(triggered()), this, SLOT(about()));

    help_action_ = new QAction(QString("键盘/鼠标控制"), this);
    connect(help_action_, SIGNAL(triggered()), this, SLOT(help()));

    //  copy_action_ = new QAction(QIcon(icon_path+"copy.png"), QString("拷贝"),
    //                             action_group_);
    //  connect(copy_action_, SIGNAL(triggered()), cloud_editor_widget_,
    //          SLOT(copy()));
    //  copy_action_->setCheckable(false);

    delete_action_ = new QAction(QIcon(icon_path+"delete.png"), QString("删除"),
                                 action_group_);
    connect(delete_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(remove()));
    delete_action_->setCheckable(false);

    //  cut_action_ = new QAction(QIcon(icon_path+"cut.png"), QString("剪切"),
    //                            action_group_);
    //  connect(cut_action_, SIGNAL(triggered()), cloud_editor_widget_,SLOT(cut()));
    //  cut_action_ -> setCheckable(false);

    //  paste_action_ = new QAction(QIcon(icon_path+"paste.png"), QString("粘贴"),
    //                              action_group_);
    //  connect(paste_action_, SIGNAL(triggered()), cloud_editor_widget_,
    //          SLOT(paste()));
    //  paste_action_ -> setCheckable(false);

    toggle_blend_action_ = new QAction(tr("Outline Points"), this);
    connect(toggle_blend_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(toggleBlendMode()));
    toggle_blend_action_->setCheckable(true);
    toggle_blend_action_->setChecked(false);

    display_z_value=new QAction(QString("显示深度值"),this);
    connect(display_z_value,SIGNAL(toggled(bool)),cloud_editor_widget_,
            SLOT(displayZValue(bool)));
    display_z_value->setCheckable(true);
    display_z_value->setChecked(false);

    display_boundary=new QAction(QString("显示边界"),this);
    connect(display_boundary,SIGNAL(toggled(bool)),cloud_editor_widget_,
            SLOT(display_boundary(bool)));
    display_boundary->setCheckable(true);
    display_boundary->setChecked(false);




    view_action_ = new QAction(QIcon(icon_path+"view.png"), QString("查看"),
                               action_group_);
    connect(view_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(view()));
    view_action_->setCheckable(true);
    view_action_->setChecked(true);

    zoom_= new QAction(QIcon(icon_path+"缩放大.png"),
                       QString("缩放"), action_group_);
    connect(zoom_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(zoom()));
    zoom_->setCheckable(true);

    interactive= new QAction(QIcon(icon_path+"compare.png"),
                             QString("比较"), action_group_);
    connect(interactive, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(interatact()));


    move= new QAction(QIcon(icon_path+"移动.png"),
                      QString("移动"), action_group_);
    connect(move, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(move()));
    move->setCheckable(true);


    undo_action_ = new QAction(QIcon(icon_path+"undo.png"),
                               QString("撤销"), action_group_);
    connect(undo_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(undo()));
    undo_action_->setCheckable(false);

    transform_action_ = new QAction(QIcon(icon_path+"move.png"),
                                    QString("变换选择"), action_group_);
    connect(transform_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(transform()));
    transform_action_->setCheckable(true);


    denoise_action_ = new QAction(QString("降噪"), this);
    connect(denoise_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(denoise()));


    crackdetect= new QAction(QString("裂缝识别"), this);
    connect(crackdetect, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(crackIdentity()));


    //  select_action_ = new QAction(QIcon(icon_path+"click.png"),
    //                               QString("点选"), action_group_);
    //  connect(select_action_, SIGNAL(triggered()), cloud_editor_widget_,
    //          SLOT(select1D()));
    //  select_action_->setCheckable(true);

    invert_select_action_ = new QAction(QIcon(icon_path+"invert.png"),
                                        QString("反选"), action_group_);
    connect(invert_select_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(invertSelect()));
    invert_select_action_->setCheckable(false);

    select_2D_action_ = new QAction(QIcon(icon_path+"select.png"),
                                    QString("框选"), action_group_);
    connect(select_2D_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(select2D()));
    select_2D_action_->setCheckable(true);

    //select_3D_action_ = new QAction(QIcon(icon_path+"cube.png"),
    //                                tr("3D Selection"), action_group_);
    //select_3D_action_->setShortcut(tr("A"));
    //connect(select_3D_action_, SIGNAL(triggered()), cloud_editor_widget_,
    //        SLOT(select3D()));
    //select_3D_action_->setCheckable(true);

    show_stat_action_ = new QAction(QIcon(icon_path+"info.png"),
                                    QString("显示统计"), action_group_);
    connect(show_stat_action_, SIGNAL(triggered()), cloud_editor_widget_,
            SLOT(showStat()));
    show_stat_action_->setCheckable(false);

    ranging_tool=new QAction(QIcon(icon_path+"ranging.png"),
                             QString("测距"),action_group_);
    connect(ranging_tool,SIGNAL(toggled(bool)),cloud_editor_widget_,
            SLOT(range(bool)));
    ranging_tool->setCheckable(true);

    extracting=new QAction(QIcon(icon_path+"extracting.png"),QString("抽取"),action_group_);
    connect(extracting,SIGNAL(triggered()),cloud_editor_widget_,SLOT(extracting()));

    extracting_save=new QAction(QIcon(icon_path+"保存.png"),QString("保存抽取文件"),action_group_);
    connect(extracting_save,SIGNAL(triggered()),cloud_editor_widget_,SLOT(saveExtractingFile()));

    SACMODEL_CIRCLE2D_action=new QAction(QString("2D圆形"),this);
    connect(SACMODEL_CIRCLE2D_action,SIGNAL(triggered()),cloud_editor_widget_,SLOT(recognize_SACMODEL_CIRCLE2D()));

    SACMODEL_CYLINDER_action=new QAction(QString("圆柱体"),this);
    connect(SACMODEL_CYLINDER_action,SIGNAL(triggered()),cloud_editor_widget_,SLOT(recognize_SACMODEL_CYLINDER()));

    //  SACMODEL_LINE_action=new QAction(QString("直线"),this);
    //  connect(SACMODEL_LINE_action,SIGNAL(toggled(bool)),cloud_editor_widget_,SLOT(recognize_SACMODEL_LINE(bool)));
    //  SACMODEL_LINE_action->setCheckable(true);

    //  SACMODEL_NORMAL_PARALLEL_PLANE_action=new QAction(QString("法线平行面"),this);
    //  connect(SACMODEL_NORMAL_PARALLEL_PLANE_action,SIGNAL(toggled(bool)),cloud_editor_widget_,SLOT(recognize_SACMODEL_NORMAL_PARALLEL_PLANE(bool)));
    //  SACMODEL_NORMAL_PARALLEL_PLANE_action->setCheckable(true);

    //  SACMODEL_NORMAL_PLANE_action=new QAction(QString("法线面"),this);
    //  connect(SACMODEL_NORMAL_PLANE_action,SIGNAL(toggled(bool)),cloud_editor_widget_,SLOT(recognize_SACMODEL_NORMAL_PLANE(bool)));
    //  SACMODEL_NORMAL_PLANE_action->setCheckable(true);

    //  SACMODEL_PARALLEL_LINE_action=new QAction(QString("平行线"),this);
    //  connect(SACMODEL_PARALLEL_LINE_action,SIGNAL(toggled(bool)),cloud_editor_widget_,SLOT(recognize_SACMODEL_PARALLEL_LINE(bool)));
    //  SACMODEL_PARALLEL_LINE_action->setCheckable(true);

    //  SACMODEL_PARALLEL_PLANE_action=new QAction(QString("平行面"),this);
    //  connect(SACMODEL_PARALLEL_PLANE_action,SIGNAL(toggled(bool)),cloud_editor_widget_,SLOT(recognize_SACMODEL_PARALLEL_PLANE(bool)));
    //  SACMODEL_PARALLEL_PLANE_action->setCheckable(true);

    //  SACMODEL_PERPENDICULAR_PLANE_action=new QAction(QString("垂直面"),this);
    //  connect(SACMODEL_PERPENDICULAR_PLANE_action,SIGNAL(toggled(bool)),cloud_editor_widget_,SLOT(recognize_SACMODEL_PERPENDICULAR_PLANE(bool)));
    //  SACMODEL_PERPENDICULAR_PLANE_action->setCheckable(true);

    SACMODEL_PLANE_action=new QAction(QString("面"),this);
    connect(SACMODEL_PLANE_action,SIGNAL(triggered()),cloud_editor_widget_,SLOT(recognize_SACMODEL_PLANE()));

    SACMODEL_SPHERE_action=new QAction(QString("球"),this);
    connect(SACMODEL_SPHERE_action,SIGNAL(triggered()),cloud_editor_widget_,SLOT(recognize_SACMODEL_SPHERE()));
    qrcode_recognize=new QAction(QString("识别二维码"),this);
    connect(qrcode_recognize,SIGNAL(triggered()),cloud_editor_widget_,SLOT(loadqrcode()));

}


void
MainWindow::createMenus ()
{
    file_menu_ = new QMenu(QString("&文件"), this);


    QFont font =file_menu_->font();
    font.setPointSize(20);
    file_menu_->setFont(font);
    //  file_menu_ -> setAttribute(Qt::WA_DeleteOnClose);
    file_menu_ -> addAction(open_action_);
    file_menu_ -> addSeparator();
    file_menu_ -> addAction(save_action_);
    file_menu_ -> addSeparator();
    file_menu_ -> addAction(exit_action_);

    edit_menu_ = new QMenu(QString("&编辑"), this);
    edit_menu_->setFont(font);
    //  edit_menu_ -> setAttribute(Qt::WA_DeleteOnClose);
    edit_menu_ -> addAction(undo_action_);
    edit_menu_ -> addSeparator();
    //  edit_menu_ -> addAction(cut_action_);
    //  edit_menu_ -> addAction(copy_action_);
    //  edit_menu_ -> addAction(paste_action_);
    edit_menu_ -> addAction(delete_action_);
    edit_menu_ -> addSeparator();
    edit_menu_ -> addAction(transform_action_);


    select_menu_ = new QMenu(QString("&选择"), this);
    select_menu_->setFont(font);
    select_menu_ -> addAction(select_2D_action_);

    display_menu_ = new QMenu(QString("&显示"), this);
    display_menu_->setFont(font);
    //  display_menu_ -> setAttribute(Qt::WA_DeleteOnClose);
    display_menu_ -> addAction(toggle_blend_action_);
    display_menu_->addAction(display_z_value);
    display_menu_->addAction(display_boundary);


    view_menu_ = new QMenu(QString("&查看"), this);
    view_menu_->setFont(font);
    //  view_menu_ -> setAttribute(Qt::WA_DeleteOnClose);
    view_menu_ -> addAction(view_action_);
    view_menu_ -> addAction(show_stat_action_);

    tool_menu_ = new QMenu(QString("&算法"), this);
    tool_menu_->setFont(font);
    //  tool_menu_ -> setAttribute(Qt::WA_DeleteOnClose);
    tool_menu_ -> addAction(denoise_action_);
    tool_menu_->addAction(crackdetect);

    help_menu_ = new QMenu(QString("&帮助"), this);
    help_menu_->setFont(font);
    //  help_menu_ -> setAttribute(Qt::WA_DeleteOnClose);
    help_menu_ -> addAction(about_action_);
    help_menu_ -> addAction(help_action_);

    recognition_menu=new QMenu(QString("&识别"),this);
    recognition_menu->setFont(font);
    recognition_menu->addAction(SACMODEL_CIRCLE2D_action);
    recognition_menu->addAction(SACMODEL_CYLINDER_action);
    //  recognition_menu->addAction(SACMODEL_LINE_action);
    //  recognition_menu->addAction(SACMODEL_NORMAL_PARALLEL_PLANE_action);
    //  recognition_menu->addAction(SACMODEL_NORMAL_PLANE_action);
    //  recognition_menu->addAction(SACMODEL_PARALLEL_LINE_action);
    //  recognition_menu->addAction(SACMODEL_PERPENDICULAR_PLANE_action);
    // recognition_menu->addAction(SACMODEL_PLANE_action);
    //recognition_menu->addAction(SACMODEL_SPHERE_action);
    recognition_menu->addAction(qrcode_recognize);


    menuBar() -> addMenu(file_menu_);
    menuBar() -> addMenu(edit_menu_);
    menuBar() -> addMenu(select_menu_);
    menuBar() -> addMenu(display_menu_);
    menuBar() -> addMenu(tool_menu_);
    menuBar()->addMenu(recognition_menu);
    menuBar() -> addMenu(help_menu_);
    menuBar()->setFont(font);
}

void
MainWindow::createToolBars ()
{
    createSpinBoxes();
    createSliders();
    view_tool_bar_ = addToolBar(tr("file"));
    view_tool_bar_->setFixedHeight(100);
    view_tool_bar_->setIconSize(QSize(60,60));
    view_tool_bar_ -> addAction(open_action_);
    view_tool_bar_ -> addAction(save_action_);
    view_tool_bar_ = addToolBar(tr("view"));
    view_tool_bar_->setFixedHeight(100);
    view_tool_bar_->setIconSize(QSize(60,60));
    view_tool_bar_ -> addAction(view_action_);
    view_tool_bar_->addAction(zoom_);
    view_tool_bar_->addAction(move);
    //  view_tool_bar_ -> addAction(select_action_);
    view_tool_bar_ -> addAction(select_2D_action_);
    view_tool_bar_->addAction(ranging_tool);
    //view_tool_bar_ -> addAction(select_3D_action_);
    view_tool_bar_ -> addAction(invert_select_action_);
    //  QLabel *ptSizeLabel = new QLabel(QString("点大小:"));
    //  ptSizeLabel -> setAttribute(Qt::WA_DeleteOnClose);
    //  view_tool_bar_ -> addWidget(ptSizeLabel);
    //  view_tool_bar_ -> addWidget(point_size_spin_box_);
    //  QLabel *selectedPtSizeLabel = new QLabel(QString("所选点大小:"));
    //  selectedPtSizeLabel -> setAttribute(Qt::WA_DeleteOnClose);
    //  view_tool_bar_ -> addWidget(selectedPtSizeLabel);
    //  view_tool_bar_ -> addWidget(selected_point_size_spin_box_);
    edit_tool_bar_ = addToolBar(tr("edit"));
    edit_tool_bar_->setFixedHeight(100);
    edit_tool_bar_->setIconSize(QSize(60,60));
    edit_tool_bar_ -> addAction(undo_action_);
    //  edit_tool_bar_ -> addAction(copy_action_);
    //  edit_tool_bar_ -> addAction(cut_action_);
    edit_tool_bar_ -> addAction(delete_action_);
    //  edit_tool_bar_ -> addAction(paste_action_);
    edit_tool_bar_ -> addAction(transform_action_);
    edit_tool_bar_ -> addAction(show_stat_action_);
    edit_tool_bar_->addAction(extracting);
    edit_tool_bar_->addAction(extracting_save);
    edit_tool_bar_->addAction(interactive);
}

void
MainWindow::increaseSpinBoxValue()
{
    point_size_spin_box_ -> setValue(point_size_spin_box_->value() + 1);
}

void
MainWindow::decreaseSpinBoxValue()
{
    point_size_spin_box_ -> setValue(point_size_spin_box_->value() - 1);
}

int
MainWindow::getSpinBoxValue()
{
    return (point_size_spin_box_->value());
}

void
MainWindow::increaseSelectedSpinBoxValue()
{
    selected_point_size_spin_box_ -> setValue(selected_point_size_spin_box_->value() + 1);
}

void
MainWindow::decreaseSelectedSpinBoxValue()
{
    selected_point_size_spin_box_ -> setValue(selected_point_size_spin_box_->value() - 1);
}

int
MainWindow::getSelectedSpinBoxValue()
{
    return (selected_point_size_spin_box_->value());
}

void
MainWindow::createSpinBoxes ()
{
    point_size_spin_box_ = new QSpinBox;
    point_size_spin_box_ -> setAttribute(Qt::WA_DeleteOnClose);
    point_size_spin_box_ -> setRange(1, 20);
    point_size_spin_box_ -> setSingleStep(1);
    point_size_spin_box_ -> setValue(2);
    connect(point_size_spin_box_, SIGNAL(valueChanged(int)),
            cloud_editor_widget_, SLOT(setPointSize(int)));
    selected_point_size_spin_box_ = new QSpinBox;
    selected_point_size_spin_box_ -> setAttribute(Qt::WA_DeleteOnClose);
    selected_point_size_spin_box_ -> setRange(1, 20);
    selected_point_size_spin_box_ -> setSingleStep(1);
    selected_point_size_spin_box_ -> setValue(10);
    connect(selected_point_size_spin_box_, SIGNAL(valueChanged(int)),
            cloud_editor_widget_, SLOT(setSelectedPointSize(int)));
}

void
MainWindow::createSliders ()
{
    move_speed_slider_ = new QSlider(Qt::Horizontal);
    move_speed_slider_ -> setAttribute(Qt::WA_DeleteOnClose);
    move_speed_slider_ -> setFocusPolicy(Qt::StrongFocus);
    move_speed_slider_ -> setTickPosition(QSlider::TicksBothSides);
    move_speed_slider_ -> setTickInterval(10);
    move_speed_slider_ -> setSingleStep(1);
}

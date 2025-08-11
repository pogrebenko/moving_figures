#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include "common/Options.h"
#include "common/IOStreamer.h"

#ifdef __SMART__
    extern CLogStream __Logger;
//    extern std::unique_ptr<CLogStream> __Logger;
    extern std::unique_ptr<CAppOption> __AppOption__;
    #define __AppOption     (*__AppOption__.get())
#else
    extern CLogStream __Logger;
    extern CAppOption __AppOption;
#endif

MainWindow::MainWindow( QWidget *parent )
: QMainWindow( parent )
, ui( new Ui::MainWindow )
, actionSave( nullptr )
, actionLoad( nullptr )
, actionExit( nullptr )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    this->setCursor(Qt::WaitCursor);

    // build menu and init toolbar
    setup_ui();
    init_ui( &__AppOption );
    // create canvas
    QHBoxLayout *pLayout = new QHBoxLayout();
    m_pCanvas = new CCanvas( this, &__AppOption, &__Logger );
    pLayout->addWidget( m_pCanvas );
    centralWidget()->setLayout( pLayout );

    this->setCursor(Qt::ArrowCursor);
}

MainWindow::~MainWindow()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    delete_ui();
}

void
MainWindow::delete_ui()
{
    delete ui;
    delete actionSave;
    delete actionLoad;
    delete actionExit;
}

void
MainWindow::setup_ui()
{
    ui->setupUi( this );

    actionSave = ui->menuFile->addAction( "Save", this, &MainWindow::on_Save );
    actionLoad = ui->menuFile->addAction( "Load", this, &MainWindow::on_Load );
    actionExit = ui->menuFile->addAction( "Exit", this, &MainWindow::on_Exit );

    connect( ui->actionRectangle, &QAction::triggered, this, &MainWindow::on_Rectangle );
    connect( ui->actionEllipce  , &QAction::triggered, this, &MainWindow::on_Ellipse   );
    connect( ui->actionTriangle , &QAction::triggered, this, &MainWindow::on_Triangle  );
    connect( ui->actionRelation , &QAction::triggered, this, &MainWindow::on_Relation  );
    connect( ui->actionDelete   , &QAction::triggered, this, &MainWindow::on_Delete    );
    connect( ui->actionMove     , &QAction::triggered, this, &MainWindow::on_Move      );

    QActionGroup* alignGroup = new QActionGroup( this );
    alignGroup->setExclusive( true );
    alignGroup->addAction( ui->actionRectangle );
    alignGroup->addAction( ui->actionEllipce   );
    alignGroup->addAction( ui->actionTriangle  );
    alignGroup->addAction( ui->actionRelation  );
    alignGroup->addAction( ui->actionDelete    );
    alignGroup->addAction( ui->actionMove      );
}

void
MainWindow::init_ui( CAppOption *pAppOption )
{
    switch( pAppOption->getActionType() )
    {
        case ActionTypeAddFigure :
        {
            switch( pAppOption->getFigureType() )
            {
                case FigureTypeTriangle  : ui->actionTriangle ->setChecked( true ); break;
                case FigureTypeEllipse   : ui->actionEllipce  ->setChecked( true ); break;
                case FigureTypeRectangle : ui->actionRectangle->setChecked( true ); break;
                case FigureTypeNone      : assert( false && "error: figure type none" ); break;
                default                  : assert( false && "error: unknown figure type" ); break;
            }
        } break;
        case ActionTypeAddRelation : ui->actionRelation->setChecked( true ); break;
        case ActionTypeDelete      : ui->actionDelete  ->setChecked( true ); break;
        case ActionTypeMove        : ui->actionMove    ->setChecked( true ); break;
        default                    : throw "error: unknown figure type"; //assert( false && "error: unknown figure type" ); break;
    }
}

void
MainWindow::on_Save()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Moving Figures File"),
        QDir::homePath(),
        tr("Moving Figures Files (*.mff);;All Files (*)")
    );
    if( !fileName.isEmpty() )
    {
        QString sError = "";
        this->setCursor( Qt::WaitCursor );
        try
        {
            QFile file( fileName );
            if( file.open( QFile::WriteOnly ) )
            {
                if( __EXPORT_FORMAT__ == ExportTypeJSON )
                {
                    QJsonObject obj; ::Write( obj, __AppOption );
                    file.write( QJsonDocument( obj ).toJson() );
                }
                else
                {
                    QByteArray Buffer;
                    CIOStreamer ios( APP_DATA_VERSION );
                    ::Write( ios, __AppOption );
                    Buffer.append( ios.m_Buffer, ios.m_Buffer_Size );
                    file.write( Buffer );
                }

                file.flush();
                file.close();

                __Logger.info( APP_LOG_LEVEL, "info: File saved [%s]", fileName.toLatin1().constData() );
            }
            else
            {
                //__Logger.error( APP_LOG_LEVEL, "error: Could not open file: %s", file.errorString().toStdString().data() );
                sError = "Error: Could not open file: %s" + file.errorString();
            }

        }
        catch( ... )
        {
            sError = "Exception error when trying to open a file.";
        }
        this->setCursor( Qt::ArrowCursor );

        if( !sError.isEmpty() )
        {
            QMessageBox msgBox( this );
                        msgBox.setIcon( QMessageBox::Critical );
                        msgBox.setText( sError );
                        msgBox.exec();
        }
    }
}

void
MainWindow::on_Load()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open Moving Figures File"),
        QDir::homePath(),
        tr("Moving Figures Files (*.mff);;All Files (*.*)")
    );

    if( !filePath.isEmpty() )
    {
        QString sError = "";
        this->setCursor(Qt::WaitCursor);
        try
        {
            QFile file( filePath );
            if( file.open( QIODevice::ReadOnly ) )
            {
                if( __EXPORT_FORMAT__ == ExportTypeJSON )
                {
                    QJsonObject obj = QJsonDocument::fromJson( file.readAll() ).object();
                    ::Read( obj, __AppOption, APP_DATA_VERSION );
                }
                else
                {
                    CIOStreamer ios( file.size(), APP_DATA_VERSION );
                    ::memcpy( ios.m_Buffer, (void*)file.readAll().begin(), file.size() );
                    ::Read( ios, __AppOption, APP_DATA_VERSION );
                }

                file.close();

                init_ui( &__AppOption );

                this->update();
                m_pCanvas->draw_figure_relation();
                m_pCanvas->update();

                __Logger.info( APP_LOG_LEVEL, "info: File loaded [%s]", filePath.toLatin1().constData() );
            }
            else
            {
                sError = "Error: Could not open file: %s" + file.errorString();

                //std::string sError = file.errorString().toStdString();
                //__Logger.error( APP_LOG_LEVEL, "error: Could not open file: %s", sError.c_str() );
            }
        }
        catch( ... )
        {
            sError = "Exception error when trying to open a file.";
        }
        this->setCursor(Qt::ArrowCursor);

        if( !sError.isEmpty() )
        {
            QMessageBox msgBox( this );
                        msgBox.setIcon(QMessageBox::Critical);
                        msgBox.setText( sError );
                        msgBox.exec();
        }
    }
}

void
MainWindow::on_Exit()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    QCoreApplication::quit();
}

void MainWindow::on_Rectangle()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    __AppOption.setFigureType(FigureTypeRectangle);
    __AppOption.setActionType(ActionTypeAddFigure);
    m_pCanvas->draw_figure_relation();
    m_pCanvas->update();
}

void MainWindow::on_Ellipse()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    __AppOption.setFigureType(FigureTypeEllipse);
    __AppOption.setActionType(ActionTypeAddFigure);
    m_pCanvas->draw_figure_relation();
    m_pCanvas->update();
}

void MainWindow::on_Triangle()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    __AppOption.setFigureType(FigureTypeTriangle);
    __AppOption.setActionType(ActionTypeAddFigure);
    m_pCanvas->draw_figure_relation();
    m_pCanvas->update();
}

void MainWindow::on_Relation()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    __AppOption.setRelationType(RelationTypeLineNondirect);
    __AppOption.setActionType(ActionTypeAddRelation);
    m_pCanvas->draw_figure_relation();
    m_pCanvas->update();
}

void MainWindow::on_Move()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    __AppOption.setActionType(ActionTypeMove);
    m_pCanvas->draw_figure_relation();
    m_pCanvas->update();
}

void MainWindow::on_Delete()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    __AppOption.setActionType(ActionTypeDelete);
    m_pCanvas->draw_figure_relation();
    m_pCanvas->update();
}

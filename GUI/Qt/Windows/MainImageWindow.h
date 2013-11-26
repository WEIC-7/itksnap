/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: Filename.cxx,v $
  Language:  C++
  Date:      $Date: 2010/10/18 11:25:44 $
  Version:   $Revision: 1.12 $
  Copyright (c) 2011 Paul A. Yushkevich

  This file is part of ITK-SNAP

  ITK-SNAP is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================*/

#ifndef MAINIMAGEWINDOW_H
#define MAINIMAGEWINDOW_H

#include <QMainWindow>
#include "SNAPCommon.h"

class GenericSliceView;
class SliceViewPanel;
class GlobalUIModel;
class QDockWidget;
class SnakeWizardPanel;
class EventBucket;
class QModelIndex;
class QProgressDialog;
class AboutDialog;

class LabelEditorDialog;
class LayerInspectorDialog;
class QtProgressReporterDelegate;
class ReorientImageDialog;
class DropActionDialog;
class MainControlPanel;
class StatisticsDialog;
class QActionGroup;

class SplashPanel;

namespace Ui {
class MainImageWindow;
}

class MainImageWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainImageWindow(QWidget *parent = 0);
  ~MainImageWindow();

  SliceViewPanel *GetSlicePanel(unsigned int i);

  // Initialize with a model
  void Initialize(GlobalUIModel *model);

  // Show the window for the first time
  void ShowFirstTime();

  // Get model
  irisGetMacro(Model, GlobalUIModel *)

  // Initiate active contour segmentation
  void OpenSnakeWizard();

  // Export a screenshot from one of the panels
  void ExportScreenshot(int panelIndex);

  // Handle drag and drop
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);

  // Get the action group for the main tool
  QActionGroup *GetMainToolActionGroup();

  // Get the action group for the 3D tool
  QActionGroup *Get3DToolActionGroup();

  // Get the layer inspector
  LayerInspectorDialog *GetLayerInspector();

  // Prompt the user whether they want to proceed with unsaved changes to
  // the segmentation and other image layers
  bool PromptForUnsavedChanges();

public slots:

  void LoadRecent(QString file);

  void AdjustMarginsForDocks();
  void onModelUpdate(const EventBucket &b);

private slots:
  void on_actionQuit_triggered();

  void on_actionLoad_from_Image_triggered();

  void on_actionImage_Contrast_triggered();

  void on_actionLabel_Editor_triggered();

  void on_actionRecent_1_triggered();
  void on_actionRecent_2_triggered();
  void on_actionRecent_3_triggered();
  void on_actionRecent_4_triggered();
  void on_actionRecent_5_triggered();

  void onSnakeWizardFinished();

  void on_listRecent_clicked(const QModelIndex &index);

  void on_actionUnload_All_triggered();


  void on_actionReorient_Image_triggered();

  void on_actionZoomToFitInAllViews_triggered();

  void on_actionCenter_on_Cursor_triggered();

  void on_actionUndo_triggered();

  void on_actionRedo_triggered();

  void on_actionOpenMain_triggered();

  void on_actionAdd_Overlay_triggered();

  void on_actionSSAxial_triggered();

  void on_actionSSCoronal_triggered();

  void on_actionSSSagittal_triggered();

  void on_actionSegmentationIncreaseOpacity_triggered();

  void on_actionSegmentationDecreaseOpacity_triggered();

  void on_actionSegmentationToggle_triggered();

  void on_actionLoadLabels_triggered();

  void on_actionSaveLabels_triggered();

  void on_actionVolumesAndStatistics_triggered();

  void on_actionSaveSegmentation_triggered();

  void on_actionSaveSegmentationAs_triggered();


  void on_actionOverlayVisibilityToggleAll_triggered();

  void on_actionOverlayVisibilityIncreaseAll_triggered();

  void on_actionOverlayVisibilityDecreaseAll_triggered();

  void on_actionLayerInspector_triggered();

  void on_actionAbout_triggered();

  void on_actionColor_Map_Editor_triggered();

  void on_actionImage_Information_triggered();

  void on_actionUnload_Last_Overlay_triggered();

  void on_actionClear_triggered();

  void on_actionSave_as_Mesh_triggered();

protected:

  bool eventFilter(QObject *obj, QEvent *event);

  void LoadDroppedFile(QString file);

private:

  void UpdateRecentMenu();
  void UpdateWindowTitle();
  void UpdateMainLayout();

  // Save the segmentation (interactively or not). Return true if save was
  // successful
  bool SaveSegmentation(bool interactive);

  // Raise a dialog (equivalent to calling show, raise, activateWindow)
  void RaiseDialog(QDialog *dialog);

  // For convenience, an array of the four panels (3 slice/1 3D)
  QWidget *m_ViewPanels[4];

  // Left and right docks
  QDockWidget *m_DockLeft, *m_DockRight;

  // Progress dialog
  QProgressDialog *m_Progress;

  // IRIS main toolbox (in left dock)
  MainControlPanel *m_ControlPanel;

  friend class QtScriptTest1;

  // SNAP wizard panel (in right dock)
  SnakeWizardPanel *m_SnakeWizard;

  Ui::MainImageWindow *ui;

  GlobalUIModel *m_Model;

  LabelEditorDialog *m_LabelEditor;

  LayerInspectorDialog *m_LayerInspector;

  ReorientImageDialog *m_ReorientImageDialog;

  DropActionDialog *m_DropDialog;

  QtProgressReporterDelegate *m_ProgressReporterDelegate;

  SplashPanel *m_SplashPanel;

  AboutDialog *m_AboutDialog;

  StatisticsDialog *m_StatisticsDialog;
};



#endif // MAINIMAGEWINDOW_H

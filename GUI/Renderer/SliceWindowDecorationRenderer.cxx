#include "SliceWindowDecorationRenderer.h"
#include "GenericSliceModel.h"
#include "SNAPAppearanceSettings.h"
#include "GlobalUIModel.h"
#include "GenericSliceRenderer.h"
#include "IRISException.h"
#include "IRISApplication.h"
#include "DisplayLayoutModel.h"

SliceWindowDecorationRenderer::SliceWindowDecorationRenderer()
{
}

SliceWindowDecorationRenderer::~SliceWindowDecorationRenderer()
{

}

void SliceWindowDecorationRenderer::paintGL()
{
  DrawOrientationLabels();
  DrawRulers();
  DrawNicknames();

}

void SliceWindowDecorationRenderer::DrawOrientationLabels()
{
  GenericSliceModel *parentModel = this->GetParentRenderer()->GetModel();
  SNAPAppearanceSettings *as =
      parentModel->GetParentUI()->GetAppearanceSettings();

  // The letter labels
  static const char *letters[3][2] = {{"R","L"},{"A","P"},{"I","S"}};
  const char *labels[2][2];

  // Get the properties for the labels
  const SNAPAppearanceSettings::Element &elt =
      as->GetUIElement(SNAPAppearanceSettings::MARKERS);

  // Leave if the labels are disabled
  if(!elt.Visible) return;

  // Repeat for X and Y directions
  for(unsigned int i=0;i<2;i++)
    {
    // Which axis are we on in anatomy space?
    unsigned int anatomyAxis =
        parentModel->GetDisplayToAnatomyTransform().GetCoordinateIndexZeroBased(i);

    // Which direction is the axis facing (returns -1 or 1)
    unsigned int anatomyAxisDirection =
        parentModel->GetDisplayToAnatomyTransform().GetCoordinateOrientation(i);

    // Map the direction onto 0 or 1
    unsigned int letterIndex = (1 + anatomyAxisDirection) >> 1;

    // Compute the two labels for this axis
    labels[i][0] = letters[anatomyAxis][1-letterIndex];
    labels[i][1] = letters[anatomyAxis][letterIndex];
    }

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  // Get the various sizes and offsets
  int offset = 4 + elt.FontSize;
  int margin = elt.FontSize / 3;
  Vector2ui vp = parentModel->GetSizeReporter()->GetViewportSize();
  int w = vp[0], h = vp[1];

  // Create the font info
  AbstractRendererPlatformSupport::FontInfo font_info =
        { AbstractRendererPlatformSupport::TYPEWRITER, elt.FontSize, true };

  // Use the delegate to draw text
  this->m_PlatformSupport->RenderTextInOpenGL(
        labels[0][0], margin, (h-offset)/2, offset, offset, font_info, -1, 0, elt.NormalColor);

  this->m_PlatformSupport->RenderTextInOpenGL(
        labels[0][1], w - (offset+margin), (h-offset)/2, offset, offset, font_info, 1, 0, elt.NormalColor);

  this->m_PlatformSupport->RenderTextInOpenGL(
        labels[1][0], (w-offset)/2, 0, offset, offset, font_info, 0, -1, elt.NormalColor);

  this->m_PlatformSupport->RenderTextInOpenGL(
        labels[1][1], (w-offset)/2, h - (offset+1), offset, offset, font_info, 0, 1, elt.NormalColor);

  glPopMatrix();
  glPopAttrib();
}

void SliceWindowDecorationRenderer::DrawNicknames()
{
  // Draw the nicknames
  GenericSliceModel *parentModel = this->GetParentRenderer()->GetModel();
  DisplayLayoutModel *dlm = parentModel->GetParentUI()->GetDisplayLayoutModel();
  Vector2ui layout = dlm->GetSliceViewLayerTilingModel()->GetValue();
  int nrows = (int) layout[0];
  int ncols = (int) layout[1];

  if(nrows * ncols == 1)
    return;

  // Get the properties for the labels
  SNAPAppearanceSettings *as =
      parentModel->GetParentUI()->GetAppearanceSettings();

  const SNAPAppearanceSettings::Element &elt =
      as->GetUIElement(SNAPAppearanceSettings::RULER);

  // Leave if the labels are disabled
  if(!elt.Visible) return;

  // Apply the label properties
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glLoadIdentity();

  SNAPAppearanceSettings::ApplyUIElementLineSettings(elt);
  glColor4d( elt.NormalColor[0], elt.NormalColor[1], elt.NormalColor[2], 1.0 );

  // Get the viewport size
  Vector2ui vp = parentModel->GetSizeReporter()->GetViewportSize();
  int w = vp[0] / ncols, h = vp[1] / nrows;

  // Find the longest nickname
  int maxtextlen = 0;
  for(int i = 0; i < nrows; i++)
    {
    for(int j = 0; j < ncols; j++)
      {
      // Define the ROI for this label
      ImageWrapperBase *layer =
          this->GetParentRenderer()->GetLayerForNthTile(i, j);
      if(layer && layer->GetNickname().length() > maxtextlen)
        maxtextlen = layer->GetNickname().length();
      }
    }

  // Adjust the font size
  int fs = elt.FontSize;
  if(fs * maxtextlen > 0.8 * w)
    fs = (int) ((1.4 * w) / maxtextlen);

  AbstractRendererPlatformSupport::FontInfo font_info =
        { AbstractRendererPlatformSupport::SANS, fs, false };

  // Draw each nickname
  for(int i = 0; i < nrows; i++)
    {
    for(int j = 0; j < ncols; j++)
      {
      // Define the ROI for this label
      ImageWrapperBase *layer =
          this->GetParentRenderer()->GetLayerForNthTile(i, j);
      if(layer)
        {
        this->m_PlatformSupport->RenderTextInOpenGL(
              layer->GetNickname().c_str(),
              w * j, h * (nrows - i) - 20, w, 15, font_info,
              AbstractRendererPlatformSupport::HCENTER,
              AbstractRendererPlatformSupport::TOP,
              elt.NormalColor);
        }
      }
    }
}

void SliceWindowDecorationRenderer::DrawRulers()
{
  GenericSliceModel *parentModel = this->GetParentRenderer()->GetModel();
  SNAPAppearanceSettings *as =
      parentModel->GetParentUI()->GetAppearanceSettings();

  // Get the properties for the labels
  const SNAPAppearanceSettings::Element &elt =
      as->GetUIElement(SNAPAppearanceSettings::RULER);

  // Leave if the labels are disabled
  if(!elt.Visible) return;

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glLoadIdentity();

  SNAPAppearanceSettings::ApplyUIElementLineSettings(elt);
  glColor4d( elt.NormalColor[0], elt.NormalColor[1], elt.NormalColor[2], 1.0 );

  // gl_font(FL_HELVETICA, elt.FontSize);

  // Pick the scale of the ruler
  Vector2ui vp = parentModel->GetSizeReporter()->GetViewportSize();

  // The ruler bar should be as large as possible but less than one half
  // of the screen width (not to go over the markers)
  double maxw = 0.5 * vp[0] - 20.0;
  maxw = maxw < 5 ? 5 : maxw;

  double zoom = parentModel->GetViewZoom();
  double scale = 1.0;
  while(zoom * scale > maxw) scale /= 10.0;
  while(zoom * scale < 0.1 * maxw) scale *= 10.0;

  // Draw a zoom bar
  double bw = scale * zoom;
  glBegin(GL_LINES);
  glVertex2d(vp[0] - 5,5);
  glVertex2d(vp[0] - 5,15);
  glVertex2d(vp[0] - 5,10);
  glVertex2d(vp[0] - (5 + bw),10);
  glVertex2d(vp[0] - (5 + bw),5);
  glVertex2d(vp[0] - (5 + bw),15);
  glEnd();

  // Based on the log of the scale, determine the unit
  string unit = "mm";
  if(scale >= 10 && scale < 1000)
    { unit = "cm"; scale /= 10; }
  else if(scale >= 1000)
    { unit = "m"; scale /= 1000; }
  else if(scale >= 1000000)
    { unit = "km"; scale /= 1000000; }
  else if(scale < 1 && scale > 0.001)
    { unit = "\xb5m"; scale *= 1000; }
  else if(scale < 0.001)
    { unit = "nm"; scale *= 1000000; }

  std::ostringstream oss;
  oss << scale << " " << unit;

  // Create the font info
  AbstractRendererPlatformSupport::FontInfo font_info =
        { AbstractRendererPlatformSupport::SANS, elt.FontSize, false };

  // See if we can squeeze the label under the ruler
  if(bw > elt.FontSize * 4)
    {
    this->m_PlatformSupport->RenderTextInOpenGL(
        oss.str().c_str(),
        vp[0]-(bw+10), 12, (int) bw, 20,
        font_info, 0, -1, elt.NormalColor);
    //gl_draw(oss.str().c_str(), 10, h - 30, (int) bw, 20, FL_ALIGN_TOP);
    }
  else
    {
    this->m_PlatformSupport->RenderTextInOpenGL(
          oss.str().c_str(),
          vp[0] - (int) (2 * bw + elt.FontSize * 4 + 20), 5, (int) (bw + elt.FontSize * 4+10), 10,
          font_info, 1, 0, elt.NormalColor);
    // gl_draw(oss.str().c_str(), (int) bw+10, h - 20, (int) bw + elt.FontSize * 4+10, 15, FL_ALIGN_LEFT);
    }

  glPopMatrix();
  glPopAttrib();

}
/*
 * spaint: TouchSelector.cpp
 */

#include "selectors/TouchSelector.h"

#include "util/CameraPoseConverter.h"

namespace spaint {

//#################### CONSTRUCTORS #################### 

TouchSelector::TouchSelector(const Settings_CPtr& settings, const TrackingState_Ptr& trackingState, const View_Ptr& view)
: PickingSelector(settings), m_touchDetector(new TouchDetector), m_trackingState(trackingState), m_view(view)
{}

//#################### PUBLIC MEMBER FUNCTIONS #################### 

Selector::Selection_CPtr TouchSelector::get_selection() const
{
  std::cout << "This is the touch selector!\n";
  return PickingSelector::get_selection();
}

void TouchSelector::update(const InputState& inputState, const RenderState_CPtr& renderState)
{
  // Run the touch pipeline.
  static boost::shared_ptr<rigging::SimpleCamera> camera(new rigging::SimpleCamera(Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero()));
  camera->set_from(CameraPoseConverter::pose_to_camera(*m_trackingState->pose_d));
  static float voxelSize = m_settings->sceneParams.voxelSize;

  m_touchDetector->run_touch_detector_on_frame(renderState, camera, voxelSize, m_view->depth);
  const TouchState& touchState = m_touchDetector->get_touch_state();

  // Update whether or not the selector is active.
  m_isActive = inputState.mouse_button_down(MOUSE_BUTTON_LEFT) || touchState.touching_surface();
  const int radius = 5;
  m_radius = radius;

  // Try and pick an individual voxel.
  m_pickPointValid = false;

  if(!touchState.touch_position_known()) return;
  int x = touchState.position_x();
  int y = touchState.position_y();

  //FIXME The following two lines are duplicated from PickingSelector.cpp
  m_pickPointValid = m_picker->pick(x, y, renderState.get(), m_pickPointFloatMB);
  if(m_pickPointValid) m_picker->to_short(m_pickPointFloatMB, m_pickPointShortMB);
}

}

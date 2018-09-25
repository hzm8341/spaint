/**
 * itmx: RenderingRequestMessage.h
 * Copyright (c) Torr Vision Group, University of Oxford, 2018. All rights reserved.
 */

#ifndef H_ITMX_RENDERINGREQUESTMESSAGE
#define H_ITMX_RENDERINGREQUESTMESSAGE

#include <ORUtils/SE3Pose.h>

#include <tvgutil/net/Message.h>

namespace itmx {

/**
 * \brief An instance of this class represents a message containing a request from a mapping client for the server to render a visualisation of the scene.
 */
class RenderingRequestMessage : public tvgutil::Message
{
  //#################### PRIVATE VARIABLES ####################
private:
  /** The byte segment within the message data that corresponds to the pose from which to render. */
  Segment m_poseSegment;

  //#################### CONSTRUCTORS ####################
public:
  /**
   * \brief Constructs a rendering request message.
   */
  RenderingRequestMessage();

  //#################### PUBLIC MEMBER FUNCTIONS ####################
public:
  /**
   * \brief Extracts the pose from which to render from the message.
   *
   * \return  The pose from which to render.
   */
  ORUtils::SE3Pose extract_pose() const;

  /**
   * \brief Copies the pose from which to render into the appropriate byte segment in the message.
   *
   * \param pose  The pose from which to render.
   */
  void set_pose(const ORUtils::SE3Pose& pose);
};

}

#endif
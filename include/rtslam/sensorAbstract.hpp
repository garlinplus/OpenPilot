/*
 * \file sensorAbstract.h
 *
 * Header file for the abstract sensor.
 *
 *  Created on: Jan 28, 2010
 *     \author: jsola
 *
 * \ingroup rtslam
 */

#ifndef SENSORABSTRACT_H_
#define SENSORABSTRACT_H_

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#include "jmath/jblas.hpp"
#include "rtslam/rtSlam.hpp"
//include parents
#include "rtslam/mapAbstract.hpp"
#include "rtslam/mapObject.hpp"
#include "rtslam/robotAbstract.hpp"

namespace jafar {
	namespace rtslam {
		using namespace std;


		// Forward declarations of children
		class ObservationAbstract;


		/**
		 * Base class for all parameter sets in module rtslam.
		 * \ingroup rtslam
		 */
		class ParametersAbstract {
			public:
				/**
				 * Mandatory virtual destructor.
				 */
				inline virtual ~ParametersAbstract(void) {
				}

		};



		/**
		 * Base class for all sensors defined in the module rtslam.
		 * \ingroup rtslam
		 */
		class SensorAbstract: public MapObject {

				friend std::ostream& operator <<(std::ostream & s, jafar::rtslam::SensorAbstract & sen);

			public:


				/**
				 * Selectable LOCAL or REMOTE pose constructor.
				 * Creates a sensor with the pose indexed in a map.
				 * \param _rob the robot
				 * \param inFilter flag indicating if the sensor state is part of the filter (REMOTE).
				 */
				SensorAbstract(const robot_ptr_t & _robPtr, const bool inFilter);

				/**
				 * Mandatory virtual destructor.
				 */
				virtual ~SensorAbstract() {
				}

				/**
				 * Parent robot
				 */
				robot_ptr_t robotPtr;

				/**
				 * A set of observations (one per landmark)
				 */
				observations_ptr_set_t observationsPtrSet;

				/**
				 * Sensor pose in robot
				 */
				Gaussian pose;

				/**
				 * Indices of sensor's global pose in map (this is either the \a ia of the robot, \a rob.ia to make it short, or the \b ia_union() of \a rob.ia and \a sen.ia)
				 */
				ind_array ia_globalPose;
				/**
				 * Flag indicating if the sensor pose is being filtered
				 */
				bool isInFilter;

			protected:
				/**
				 * Sensor parameters.
				 * Derive this class for real sensors,
				 * and use dynamic down-cast in the associated observation classes to access the derived parameters.
				 */
				ParametersAbstract* paramsAbs;

			public:


				void linkToObservation(const observation_ptr_t & _obsPtr); ///< Link to observation
				void linkToRobot(const robot_ptr_t & _robPtr); ///<             Link to robot

				/*
				 * Acquire raw data.
				 */
				virtual void acquireRaw();

				/**
				 * Process raw data.
				 */
				virtual void processRaw();

			protected:
				/**
				 * Observe known landmarks
				 */
				virtual void observeKnownLmks();
				/**
				 * Discover new landmarks.
				 * This function explores the raw data to find new features and use them for landmark initialization.
				 */
				virtual void discoverNewLmks();

				/**
				 * Add one landmark to the map.
				 * TODO: see if we move this to MapAbstract -> lmkPtr = mapPtr->newLandmark(senPtr)
				 * TODO: need to solve first the pointer-from-this issue of shared_ptr.
				 * \param mapPtr pointer to the slam map.
				 */
				landmark_ptr_t newLandmark(map_ptr_t & mapPtr);


			public:


				/**
				 * Get sensor pose in global frame.
				 * This function composes robot pose with sensor pose to obtain the global sensor pose.
				 * It renders the Jacobians of the composed frame wrt all variables that are in the map (either robot only, or robot and sensor),
				 * depending on the sensor pose storage being LOCAL or REMOTE (see class Gaussian for doc on storage options).
				 * Therefore, this Jacobian is either 7-by-7 (LOCAL sensor pose) or 7-by-14 (REMOTE sensor pose).
				 *
				 * The concerned states are available as an indirect array \a ia_rs, member of the class and defined at construction time.
				 * \param poseG the global pose.
				 * \param PG_rs the Jacobian wrt the mapped states of robot and sensor.
				 */
				void globalPose(jblas::vec7 & senGlobalPose, jblas::mat & SG_rs);

		};

	}
}

#endif /* SENSORABSTRACT_H_ */

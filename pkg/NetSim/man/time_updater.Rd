\name{time_updater}
\alias{create_timer_updater}
\docType{package}
\title{
	Overview of time updaters implemented in NetSim
}
\description{
	Overview of time updaters implemented in NetSim. So far, there is one time updater
	that keeps a global timer up-to-date by adding time spans determined by time models.
}
\usage{
	create_timer_updater(timerIndex)
}
\arguments{
	\item{timerIndex}{A pointer to a global attribute in the process state representing the timer}
}
\seealso{
	\code{\link{create_model_manager}}
}
\examples{
	processState <- create_process_state()
	
	# add timer variable to process state
	processState <- add_global_attribute(processState, name = "timer", value = 0)
	timerIndex <- get_global_attribute_index(processState, name = "timer")
	
	# create time updater based on the timer index
	timeUpdater <- create_timer_updater(timerIndex)

}
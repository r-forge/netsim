\name{simulator}
\alias{create_simulator}
\alias{simulate}
\docType{package}
\title{
	Functions related to run a simulation 
}
\description{
	Functions related to run a simulation based on a process state and a model manager.
}
\usage{
	create_simulator(processState, modelManager, timeSpan = 1, verbose = TRUE, debug = FALSE)
	simulate(simulator)
}
\arguments{
	\item{processState}{A process state object}
	\item{modelManager}{A model manager object}
	\item{timeSpan}{A time span to simulate. The default is 1.}
	\item{verbose}{If TRUE it generates some basic information output including a progress bar}
	\item{debug}{If TRUE it generate a very detailed output for some models. Slows down the simulation process significantly.}
	\item{simulator}{A simulator object}
}
\seealso{
	\code{\link{create_process_state}}
	\code{\link{create_model_manager}}
}
\examples{
	## Detailed examples of process states and model managers can be found in 
	## section 4 of the package vignette.
	processState <- create_process_state()
	modelManager <- create_model_manager()
	
	timeSpan <- 20
	simulator <- create_simulator(processState, modelManager, timeSpan,
	verbose = TRUE, debug = FALSE)
	simulate(simulator)
}
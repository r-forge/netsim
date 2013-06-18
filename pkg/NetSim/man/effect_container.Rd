\name{effect_container}
\alias{create_effect_container}
\alias{add_to_effect_container}
\alias{create_effect}
\alias{add_effect}
\alias{get_effect_type}
\docType{package}
\title{
	Overview of SAOM-related functions
}
\description{
	Overview of functions to specify stochastic actor-oriented models. The naming of
	effects is in line with the RSiena manual (see reference below). 
	
		The following effects are implemented in SAOMs for NetSim so far (to be extended, see RSiena manual for an explanation):\cr
	\emph{OneModeNetworkEffect} \cr
	(initialized with paraeter: networkIndex)\cr
	- density \cr
	- recip \cr
	- transTrip \cr
	- cycle3 \cr
	- nbrDist2 \cr
	- inPop \cr
	- outPop \cr
	\emph{AttributeOneModeNetworkEffect} \cr
	(initialized with parameters attributeIndex, networkIndex)\cr
	- altX \cr
	- egoX \cr
	\emph{SimilarityAttributeOneModeNetworkEffect} \cr
	(initialized with parameters attributeIndex, networkIndex, meanSimilarityScore) \cr
	- simX \cr
	- totSim \cr
	\emph{AttributeEffect} \cr
	(initialized with parameter attributeIndex)
	- linear \cr
	- quad \cr
	\emph{MultiplexNetworkEffect}
	(initialized with parameters networkIndex, networkIndex1, networkIndex2) \cr
	- crprod	
}
\usage{
	create_effect_container()
	add_to_effect_container(effectContainer, effect, parameter)
	create_effect(effectName, ...)
	add_effect(changeModel, effect, attributeIndex)
	get_effect_type(effectName)
}
\arguments{
	\item{effectContainer}{An effect container object}
	\item{effect}{An effect object created with create_effects function}
	\item{parameter}{Parameter of the effect specified}
	\item{effectName}{String name of the effect according to the SIENA manual}
	\item{changeModel}{A SAOM change model}
	\item{...}{Additional parameters depending on the type of the effect (see description above)}
	\item{attributeIndex}{Index of an attribute containing the (changeable) parameter}
}
\seealso{
	\code{\link{change_models}}
	\code{\link{create_tie_swap_updater}}
}
\references{
Ripley, R. M.; Snijders, T. A. B. & Preciado Lopez, P.: Manual for SIENA 4.0. (2012), 
Oxford: University of Oxford, Department of Statistics; Nuffield College. 
URL: http://www.stats.ox.ac.uk/~snijders/siena/RSiena_Manual.pdf
}
\examples{
## See sections 4.1, 4.2 and 4.5 in the package vignette for detailed examples
## Small example:

}
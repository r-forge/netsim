\name{package-integration}
\alias{as.igraph}
\docType{package}
\title{
	Functions to transform NetSim objects to objects of other network classes
}
\description{
	Functions to transform NetSim objects to objects of other network classes.
	So far, a transformation function to igraph objects is implemented	
}
\usage{
	\method{as.igraph}{NetSimNetwork}(x, mode=c("directed", "undirected", ...)
}
\arguments{
	\item{x}{A Netsim network object}
	\item{mode}{Character scalar, specifies whether igraph should interpret the graph
	as directed or undirected. More options are not supported, yet.}
}
\references{
Csardi G, Nepusz T: The igraph software package for complex network research, InterJournal, Complex Systems 1695. 2006. 
\url{http://igraph.sf.net}
}
\seealso{
}
\examples{
	nActors <- 4
	network <- create_network(matrix(0, nActors, nActors))

	# set ties
	set_tie(network, i = 0, j = 1, value = 1)
	set_tie(network, i = 0, j = 2, value = 1)
	
	library(igraph)
	myIGraph <- as.igraph(network, mode = "directed")
	plot(myIgraph) # now using the igraph package
}
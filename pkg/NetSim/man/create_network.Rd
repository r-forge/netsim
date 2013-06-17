\name{create_network}
\alias{create_network}
\alias{set_tie}
\alias{network_as_matrix}
\alias{as.matrix.NetSimNetwork}
\docType{package}
\title{
	Create an network object and maintain it
}
\description{
	Create a network object in \pkg{NetSim} from a matrix, set particular ties of the network and 
	transform it back into a matrix
}
\usage{
	create_network(matrix, directed = TRUE, reflexive = FALSE)
	set_tie(network, i, j, value)
	network_as_matrix(network)
	as.matrix(network)
}
\arguments{
	\item{matrix}{A (squared) matrix object}
	\item{directed}{Indicates whether the network is directed (TRUE) or undirected (FALSE)}
	\item{reflexive}{Indicates whether the matrix is reflexive (TRUE) or not (FALSE)}
	\item{network}{A \pkg{NetSim} network object}
	\item{i,j}{index of one particular tie. The index starts counting from 0.}
	\item{value}{A value a tie is set to (1 or 0 in binary networks)}
}
\seealso{
	\code{\link{process_state}}
	\code{\link{create_attribute_container}}
	\code{\link{network_utils}}
}
\examples{
nActors <- 5
network <- create_network(matrix(1, nActors, nActors))

# set ties
set_tie(network, i = 0, j = 1, value = 0)
set_tie(network, i = 0, j = 2, value = 0)
# returns FALSE and keeps the network unchanged as the network is not reflexive
set_tie(network, i = 0, j = 0, value = 1)

# there are two equivalent ways to re-transform the network into a matrix
as.matrix(network)
network_as_matrix(network)
}
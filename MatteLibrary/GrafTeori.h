#pragma once
#include <vector>
#include <utility>
#include <unordered_map>
class WeightedGraph
{
private:
public:
	std::vector<int> Vertices = std::vector<int>(0);
	WeightedGraph(int NumberOfVertices);
	~WeightedGraph();
	//implementation med n^2 minnes komplexitet
	std::unordered_map<int, std::vector<std::pair<std::pair<int, int>, double>>> Edges;
	long double ShortestPath(int StartNode, int EndNode);
	long double MatrixShortestPathToRightColumn(int StartNode, int NumberOfColumns, int NumberOfRows);
	void AddEdge(std::pair<int, int> Edge, long double Value);
	void AddEdgeDirected(std::pair<int, int> Edge, long double Value);
};
#define DINFINITY (double)9223372036854775807
long double WeightedGraph::ShortestPath(int StartNode, int EndNode)
{
	//djikstars algortim
	int CurrentNode = StartNode;
	std::vector<bool> UnvisitedNodes = std::vector<bool>(Vertices.size());
	for (size_t i = 0; i < Vertices.size(); i++)
	{
		UnvisitedNodes[i] = false;
	}
	std::vector<long double> NodeDistances = std::vector<long double>(UnvisitedNodes.size());
	int NodeDistanceSize = NodeDistances.size();
	for (int i = 0; i < NodeDistanceSize; i++)
	{
		if (i != StartNode)
		{
			//borde implementera long double infinity
			NodeDistances[i] = DINFINITY;
		}
		else
		{
			NodeDistances[i] = 0;
		}
	}
	//steg 1 och 2 avklarat
	while (true)
	{
		//går igenom visisted nodesens unvisited grannar
		//steg 3
		for (int i = 0; i < Edges[CurrentNode].size(); i++)
		{
			//vi kollar om noden vi får av kanten är visited
			int NeighbourToCompare = Edges[CurrentNode][i].first.second;
			if (UnvisitedNodes[NeighbourToCompare] == false)
			{
				//vi kollar om tentative steppet ger ett mindre värde
				if (NodeDistances[CurrentNode] + Edges[CurrentNode][i].second < NodeDistances[NeighbourToCompare])
				{
					NodeDistances[NeighbourToCompare] = NodeDistances[CurrentNode] + Edges[CurrentNode][i].second;
				}
			}
		}
		//steg 4
		UnvisitedNodes[CurrentNode] = true;
		//steg 5
		if (UnvisitedNodes[EndNode] == true)
		{
			break;
		}
		//steg 6, väluj den unvisited node med mist tentative distance och gör den till current
		long double MinValue = DINFINITY;
		int UnvisitedNodesSize = UnvisitedNodes.size();
		for (int i = 0; i < UnvisitedNodesSize; i++)
		{
			if (UnvisitedNodes[i] == false)
			{
				if (NodeDistances[i] < MinValue)
				{
					MinValue = NodeDistances[i];
					CurrentNode = i;
				}
			}
		}
		if (MinValue == DINFINITY)
		{
			break;
		}
	}
	return(NodeDistances[EndNode]);
}
void WeightedGraph::AddEdge(std::pair<int, int> Edge, long double Value)
{
	//vi lägger till kopplingen för varje hörn, och ser till att värdet till vänster är själva hörnet vi går från
	//kollar att vi inte redan har ett hörn, vi throwar inget error utan lägger inte bara till den
	for (int i = 0; i < Edges[Edge.first].size(); i++)
	{
		if (Edges[Edge.first][i].first.second == Edge.second)
		{
			return;
		}
	}
	Edges[Edge.first].push_back(std::pair<std::pair<int, int>, long double>(Edge, Value));
	Edges[Edge.second].push_back(std::pair<std::pair<int, int>, long double>(std::pair<int, int>(Edge.second, Edge.first), Value));
}
WeightedGraph::WeightedGraph(int NumberOfVertices)
{
	Vertices = std::vector<int>(NumberOfVertices);
	for (int i = 0; i < NumberOfVertices; i++)
	{
		Edges[i] = std::vector<std::pair<std::pair<int, int>, double>>(0);
	}
}
WeightedGraph::~WeightedGraph()
{

}
void WeightedGraph::AddEdgeDirected(std::pair<int, int> Edge, long double Value)
{
	Edges[Edge.first].push_back(std::pair<std::pair<int, int>, long double>(Edge, Value));
}
long double WeightedGraph::MatrixShortestPathToRightColumn(int StartNode, int NumberOfColumns, int NumberOfRows)
{
	//djikstars algortim
	int CurrentNode = StartNode;
	std::vector<bool> UnvisitedNodes = std::vector<bool>(Vertices.size());
	for (size_t i = 0; i < Vertices.size(); i++)
	{
		UnvisitedNodes[i] = false;
	}
	std::vector<long double> NodeDistances = std::vector<long double>(UnvisitedNodes.size());
	int NodeDistanceSize = NodeDistances.size();
	for (int i = 0; i < NodeDistanceSize; i++)
	{
		if (i != StartNode)
		{
			//borde implementera long double infinity
			NodeDistances[i] = DINFINITY;
		}
		else
		{
			NodeDistances[i] = 0;
		}
	}
	//steg 1 och 2 avklarat
	while (true)
	{
		//går igenom visisted nodesens unvisited grannar
		//steg 3
		for (int i = 0; i < Edges[CurrentNode].size(); i++)
		{
			//vi kollar om noden vi får av kanten är visited
			int NeighbourToCompare = Edges[CurrentNode][i].first.second;
			if (UnvisitedNodes[NeighbourToCompare] == false)
			{
				//vi kollar om tentative steppet ger ett mindre värde
				if (NodeDistances[CurrentNode] + Edges[CurrentNode][i].second < NodeDistances[NeighbourToCompare])
				{
					NodeDistances[NeighbourToCompare] = NodeDistances[CurrentNode] + Edges[CurrentNode][i].second;
				}
			}
		}
		//steg 4
		UnvisitedNodes[CurrentNode] = true;
		//steg 5
		if (UnvisitedNodes[Vertices.size() - 1] == true)
		{
			break;
		}
		//steg 6, väluj den unvisited node med mist tentative distance och gör den till current
		long double MinValue = DINFINITY;
		int UnvisitedNodesSize = UnvisitedNodes.size();
		for (int i = 0; i < UnvisitedNodesSize; i++)
		{
			if (UnvisitedNodes[i] == false)
			{
				if (NodeDistances[i] < MinValue)
				{
					MinValue = NodeDistances[i];
					CurrentNode = i;
				}
			}
		}
		if (MinValue == DINFINITY)
		{
			break;
		}
	}
	long double ReturVärdet = DINFINITY;
	for (int i = 1; i <= Vertices.size() / NumberOfRows; i++)
	{
		//vi går igenom varje tal i kolumnen och kollar vilket som har lägst värde
		assert(NodeDistances[(i * NumberOfColumns) - 1] != DINFINITY);
		if (NodeDistances[(i * NumberOfColumns) - 1] < ReturVärdet)
		{
			ReturVärdet = NodeDistances[(i * NumberOfColumns) - 1];
		}
	}
	return(ReturVärdet);
}
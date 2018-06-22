#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <climits>

//Global structure to store the id and coordinates of a city
//http://www.learncpp.com/cpp-tutorial/47-structs/
struct CITY{
	int id;
	int x;
	int y;
};




//function to read contents from the file
void readFile(std::string fname, std::vector<struct CITY>& cities) 
{
	//create file stream to read from
	std::ifstream ifs;

	//open the file for reading
	ifs.open(fname.c_str());

	//check for opening errors, if error, exit program
	if(!ifs.is_open())
	{
		std::cout << "\nError opening file: " << fname << "\n" 
			<< std::endl;
			exit(1);
	}

	//read file contents into proper places
	struct CITY newCity;	//to store the city contents on each line
	int a, b, c;	//for each line's content
	while(ifs >> a >> b >> c)
	{
			
		newCity.id = a;
		newCity.x = b;
		newCity.y = c;
	
	//put the newCity into the cities vector
		cities.push_back(newCity);
	}	

	//close the file
	ifs.close();
}




// Description: calculates the distance between two points
int distance(int x1, int y1, int x2, int y2)
{
	int dx = x1 - x2;
	int dy = y1 - y2;
	
	return int(round(sqrt(dx*dx + dy*dy)));
}




// Description: converts a vector of cities to an adjacency matrix
std::vector< std::vector<int> > createAdjMatrix(std::vector<struct CITY>& cities)
{
	const int nCities = cities.size();
	std::vector< std::vector<int> > result(nCities, std::vector<int> (nCities, 0));
	
	//convert data to adjacency matrix
	for (int i = 0; i < nCities; i++)
		for (int j = 0; j < nCities; j++)
			result[i][j] = distance(cities[i].x, cities[i].y, cities[j].x, cities[j].y);
	
	return result;
}




// Description: finds the two cities closest to each other
std::vector<int> getClosestCities(std::vector< std::vector<int> > cities)
{
	const int nCities = cities.size();			// number of cities
	std::vector<int> result(2, 0);				// result to store the 2 cities
	int minDistance = INT_MAX;
	
	for (int i = 0; i < nCities; i++)
	{
		for (int j = 0; j < nCities; j++)
		{
			if (cities[i][j] < minDistance && i != j)
			{
				minDistance = cities[i][j];
				result[0] = i;
				result[1] = j;
			}
		}
	}
	
	return result;
}




// Description: modifies adjacency matrix to account for the loneliness of near cities 
std::vector< std::vector<int> > modifyAdjMatrix(std::vector<std::vector<int> > cities)
{
	const int nCities = cities.size();
	std::vector<int> dist(nCities, 0);
	std::vector< std::vector<int> > result(nCities, std::vector<int> (nCities, 0));
	
	// calculate total distance to each other city
	for (int i = 0; i < nCities; i++)
		for (int j = 0; j < nCities; j++)
			dist[i] += cities[i][j];
	
	// calculate min, max, and average distance of each city to all other cities
	int min = dist[0];
	int max = dist[0];
	for (int i = 0; i < nCities; i++)
	{
		if (dist[i] < min)
			min = dist[i];
		if (dist[i] > max)
			max = dist[i];
	}
	int avg = (min + max) / 2;
	
	// update distances so that larger distances (in comparison to the average)
	// are proportional to smaller distances
	for (int i = 0; i < nCities; i++)
	{
		if (dist[i] > avg)
			dist[i] = avg - (dist[i] - avg);
		else
			dist[i] = avg + (avg - dist[i]);
	}	
	
	// create new matrix
	for (int i = 0; i < nCities; i++)
		for (int j = 0; j < nCities; j++)
			result[i][j] = ((nCities * cities[i][j]) + dist[j]) / 2;
	
	return result;
}




// Description: calculates tour length
int tourLength(std::vector< std::vector<int> > cities, std::vector<int> tour)
{
	int total = 0;
	for (int i = 0; i < tour.size() - 1; i++)
		total += cities[tour[i]][tour[i + 1]];
	total += cities[tour[0]][tour[tour.size() - 1]];	
	return total;
}




// Description: creates a tour by finding the nearest city to the one being examined
std::vector<int> nearestNeighbor(std::vector< std::vector<int> > cities, int startingCity)
{
	std::vector<bool> visited(cities.size(), false);	// list to keep track of cities already visited
	std::vector<int> tour;					// list that keeps track of tour
	int u = startingCity;					// most recent city added to tour
	int v = 0;						// city about to be added to tour
	
	// mark first city as visited and add it to the tour
	visited[startingCity] = true;
	tour.push_back(startingCity);
	
	// populate tour until all cities are included
	while (tour.size() != cities.size())
	{
		int minDistance = INT_MAX;			// smallest distance between cities
		
		// loop through all cities to find the nearest city
		for (int i = 0; i < visited.size(); i++)
		{
			// add city to tour if it is the nearest city that has not yet been visited
			if (cities[u][i] < minDistance && !visited[i])
			{
				minDistance = cities[u][i];
				v = i;
			}
		}
		u = v;
		tour.push_back(v);
		visited[v] = true;
	}

	return tour;
}





int main(int argc, char *argv[])
{
	//check for input file
	//http://www.cplusplus.com/articles/DEN36Up4/
	if(argc != 2){
		std::cout << "\nYou must pass an input file to the program.\n" << 
			std::endl;
		exit(1);
	}
	
	//store file name in variable to pass reading function
	std::string fileName = argv[1];

	//create vector to store the contents of the file
	std::vector<struct CITY> cities;
	
	//call function to read the lines in from the file
	readFile(fileName, cities);
	
	//create vectors to hold the adjacency matrices
	std::vector< std::vector<int> > adjMatrix;
	std::vector< std::vector<int> > newAdjMatrix;

	//fill the matrices with distacnes between all cities
	adjMatrix = createAdjMatrix(cities);
	newAdjMatrix = modifyAdjMatrix(adjMatrix);

	//find the cities that are closest to each other to use as start points
	std::vector<int> startingCities = getClosestCities(adjMatrix);

	//populte our tour vector with the order of visited cities
	std::vector<int> NL_Tour = nearestNeighbor(newAdjMatrix, startingCities[0]);

	// total distance
	int tourTotal = tourLength(adjMatrix, NL_Tour);	
	
	// print tour length results to the console
	std::cout << "\nNearest and Loneliest Neighbor tour length: " << tourTotal << "\n" << std::endl;
	
	//print the tour length and cities to a file
	//create file stream to read from
	std::ofstream ofs;

	//open the file for writing
	ofs.open((fileName + ".tour").c_str());
	
	//check for opening errors, if error, exit program
	if(!ofs.is_open())
	{
		std::cout << "\nError opening file for writing: " << fileName 
			<< ".tour" << "\n" << std::endl;
			exit(1);
	}

	//print distance to the file
	ofs << tourTotal<< "\n";

	//print cities to the file
	for(int i = 0; i < NL_Tour.size(); i++)
	{
		ofs << NL_Tour[i] << "\n";
	}	

	//close the output file
	ofs.close();

	return 0;
}

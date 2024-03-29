#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

static std::map<int, std::string> cookbookMap;

class Time
{
	public:
		int mins_;
		int hours_;
	
		void validate()
		{
			if (abs(mins_) >= 60)
			{
				hours_ += mins_ / 60;
				mins_ %= 60;
			}
		}

		Time() 
		{
			mins_ = 0;
			hours_ = 0;
		}

		Time (int m)
		{
			mins_ = m;
			hours_ = 0;
			validate();
		}

		Time (int m, int h)
		{
			mins_ = m;
			hours_ = h;
			validate();
		}

		std::string toString()
		{
			std::string str = "";
			
			if (hours_ > 0)
			{
				str += std::to_string(hours_) + " hour";
				if (hours_ > 1)
				{
					str += "s";
				}
			}

			if (mins_ > 0 && hours_ > 0)
			{
				str += " and ";
			}

			if (mins_ > 0)
			{
				str += std::to_string(mins_) + " min";
				if (mins_ > 1)
				{
					str += "s";
				}
			}
			
			return str;
		}
};

class Recipe
{
	public:
		std::string name_;
		Time prepTimeMin_;
		Time cookTimeMin_;
		int cookbookNum_;
		std::string cookbookName_;
		int cookbookPage_;

		Recipe() {}

		std::string getName()
		{
			return name_;
		}

		std::string getPrepTime()
		{
			return prepTimeMin_.toString();
		}

		std::string getCookTime()
		{
			return cookTimeMin_.toString();
		}

		std::string toString()
		{
			std::string ret = "";

			ret += name_ + ".";
			ret += " Prep: " + getPrepTime() + ".";
			ret += " Cook: " + getCookTime() + ".";

			if (cookbookNum_ == 0)
			{
				ret += " Recipe number " + std::to_string(cookbookPage_);
				ret += " in personal recipe list";
			}
			else
			{
				ret += " Page " + std::to_string(cookbookPage_);
				ret += " in " + cookbookName_;
			}

			return ret + ".";
		}
};

void setupCookbookMap()
{
	cookbookMap[0] = "Personal Recipe List";
	cookbookMap[1] = "The Easy Kitchen - 150 Great Recipes";
	cookbookMap[2] = "Easy Cookbook - 175 Winning Recipes";
}

// Thank you chatGPT :)
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

Recipe parseRecipe(std::vector<std::string> item)
{
	Recipe recipe;
	recipe.name_ = item[0];
	recipe.prepTimeMin_ = Time(stoi(item[1]));
	recipe.cookTimeMin_ = Time(stoi(item[2]));
	recipe.cookbookNum_ = stoi(item[3]);
	recipe.cookbookName_ = cookbookMap[recipe.cookbookNum_];
	recipe.cookbookPage_ = stoi(item[4]);
	return recipe;
}

void readFile(std::vector<Recipe> &recipes)
{
	std::ifstream input("Recipes.csv");

	std::string line;
	getline(input, line); // ignore first line

	for (; getline(input, line); )
	{
		recipes.push_back(parseRecipe(split(line, ',')));
	}
}

void printAllRecipes(std::vector<Recipe> recipes)
{
	for (Recipe recipe : recipes)
	{
		std::cout << recipe.toString() << std::endl;
	}
}

int main()
{
	setupCookbookMap();

	std::vector<Recipe> recipes;
	readFile(recipes);

	printAllRecipes(recipes);
}
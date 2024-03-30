#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>

static std::map<int, std::string> cookbookMap;
static std::map<int, std::string> mealTypeMap;

enum cookbooks
{
	PERSONAL,
	THE_EASY_KITCHEN,
	EASY_COOKBOOK
};

enum mealTypes
{
	MEAL,
	SNACK,
	DESSERT
};

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

		bool isNonZero()
		{
			return !(mins_ == 0 && hours_ == 0);
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
		bool dataSet_;
		std::string name_;
		Time prepTime_;
		Time cookTime_;
		int cookbookNum_;
		std::string cookbookName_;
		int cookbookPage_;
		int mealTypeNum_;
		std::string mealTypeName_;

		Recipe() 
		{
			dataSet_ = false;
		}

		std::string getPrepTime()
		{
			return prepTime_.toString();
		}

		std::string getCookTime()
		{
			return cookTime_.toString();
		}

		std::string toString()
		{
			std::string ret = "";

			ret += name_ + ".\n";

			if (prepTime_.isNonZero())
			{
				ret += "Prep time: " + getPrepTime() + ". ";
			}
	
			ret += "Cook time: " + getCookTime() + ".\n";

			if (cookbookNum_ == 0)
			{
				ret += "Recipe number " + std::to_string(cookbookPage_);
				ret += " in personal recipe list";
			}
			else
			{
				ret += "Page " + std::to_string(cookbookPage_);
				ret += " in " + cookbookName_;
			}

			ret += ".\nType: " + mealTypeName_;

			return ret + ".";
		}
};

void setupMaps()
{
	cookbookMap[PERSONAL] = "Personal Recipe List";
	cookbookMap[THE_EASY_KITCHEN] = "The Easy Kitchen - 150 Great Recipes";
	cookbookMap[EASY_COOKBOOK] = "Easy Cookbook - 175 Winning Recipes";

	mealTypeMap[MEAL] = "meal";
	mealTypeMap[SNACK] = "snack";
	mealTypeMap[DESSERT] = "dessert";
}

// Thank you chatGPT :)
std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	
	return tokens;
}

void parseRecipe(std::vector<std::string> item, Recipe &recipe)
{
	if (item.size() > 5)
	{
		recipe.name_ = item[0];
		recipe.prepTime_ = Time(stoi(item[1]));
		recipe.cookTime_ = Time(stoi(item[2]));
		recipe.cookbookNum_ = stoi(item[3]);
		recipe.cookbookName_ = cookbookMap[recipe.cookbookNum_];
		recipe.cookbookPage_ = stoi(item[4]);
		recipe.mealTypeNum_ = stoi(item[5]);
		recipe.mealTypeName_ = mealTypeMap[recipe.mealTypeNum_];
		recipe.dataSet_ = true;
	}
}

void readFile(std::vector<Recipe> &recipes)
{
	std::ifstream input("Recipes.csv");

	std::string line;
	getline(input, line); // ignore first line

	for (; getline(input, line); )
	{
		Recipe recipe;
		parseRecipe(split(line, ','), recipe);
		if (recipe.dataSet_)
		{
			recipes.push_back(recipe);
		}
	}
}

void printAllRecipes(std::vector<Recipe> recipes)
{
	for (Recipe recipe : recipes)
	{
		std::cout << recipe.toString() << std::endl << std::endl;
	}
}

std::vector<Recipe> getListBasedOnArgs(std::vector<Recipe> recipes, bool meal, bool snack, bool dessert, int book)
{
	if (meal && snack && dessert && book == -1)
	{
		return recipes; // no point filtering
	}

	std::vector<Recipe> filteredRecipes;
	for (Recipe recipe : recipes)
	{
		bool correctType = (meal && recipe.mealTypeNum_ == MEAL)
							|| (snack && recipe.mealTypeNum_ == SNACK)
							|| (dessert && recipe.mealTypeNum_ == DESSERT);

		if (correctType && (book == -1 || recipe.cookbookNum_ == book))
		{
			filteredRecipes.push_back(recipe);
		}
	}
	return filteredRecipes;
}

// Thank you chatGPT :)
int getRandomInt(int max) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis(0, max);
	return dis(gen);
}

void printRandomRecipe(std::vector<Recipe> recipes)
{
	std::cout << recipes[getRandomInt(recipes.size())].toString() << std::endl;
}

int main(int argc, char **argv)
{
	setupMaps();

	std::vector<Recipe> recipes;
	readFile(recipes);

	bool typeSet = false;
	bool meal = false;
	bool snack = false;
	bool dessert = false;
	int book = -1;
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-m")
		{
			meal = true;
			typeSet = true;
		}
		else if (arg == "-s")
		{
			snack = true;
			typeSet = true;
		}
		else if (arg == "-d")
		{
			dessert = true;
			typeSet = true;
		}
		else if (arg == "-b")
		{
			if (i + 1 < argc) {
				book = std::atoi(argv[i + 1]);
				++i;
			} else {
				std::cerr << "Error: Argument missing after -b" << std::endl;
				return 1;
			}
		}
	}

	if (!typeSet)
	{
		meal = true;
		snack = true;
		dessert = true;
	}

	std::vector<Recipe> filteredRecipes = getListBasedOnArgs(recipes, meal, snack, dessert, book);
	// printAllRecipes(recipes);

	if (filteredRecipes.size() == 0)
	{
		std::cout << "No recipes match the following filters:";
		if (meal)
		{
			std::cout << " meals";
		}
		if (snack)
		{
			std::cout << " snacks";
		}
		if (dessert)
		{
			std::cout << " desserts";
		}
		if (book != -1)
		{
			std::cout << " cookbook " << book;
		}
		std::cout << "." << std::endl;
	}
	else
	{
		printRandomRecipe(filteredRecipes);
	}
}
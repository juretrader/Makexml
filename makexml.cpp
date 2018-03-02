#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

//////////////////////////////////////////////////////////
// Schemes - define your scheme like this
//////////////////////////////////////////////////////////
enum Doh_KDVP_elements
{
	Doh_KDVP_element_KDVPItem = 0,
	Doh_KDVP_element_InventoryListType,
	Doh_KDVP_element_Name,
	Doh_KDVP_element_HasForeignTax,
	Doh_KDVP_element_HasLossTransfer,
	Doh_KDVP_element_ForeignTransfer,
	Doh_KDVP_element_TaxDecreaseConformance,
	Doh_KDVP_element_Securities,
	Doh_KDVP_element_SecuritiesShort,
	Doh_KDVP_element_Code,
	Doh_KDVP_element_IsFond,
	Doh_KDVP_element_Row,
	Doh_KDVP_element_ID,
	Doh_KDVP_element_Purchase,
	Doh_KDVP_element_Sale,
	Doh_KDVP_element_F1,
	Doh_KDVP_element_F2,
	Doh_KDVP_element_F3,
	Doh_KDVP_element_F4,
	Doh_KDVP_element_F6,
	Doh_KDVP_element_F7,
	Doh_KDVP_element_F8,
	Doh_KDVP_element_F9,
	Doh_KDVP_element_F10,

	Doh_KDVP_element_last
};

enum Doh_KDVP_line_values
{
	Doh_KDVP_line_values_ticker = 0,
	Doh_KDVP_line_values_trans_id,
	Doh_KDVP_line_values_trans_type,
	Doh_KDVP_line_values_date,
	Doh_KDVP_line_values_shares,
	Doh_KDVP_line_values_price,
	Doh_KDVP_line_values_loss_valid,

	Doh_KDVP_line_values_last
};

//////////////////////////////////////////////////////////
// General scheme data - common for all schemes
//////////////////////////////////////////////////////////

enum tagType
{
	tagTypeGroup = 0,	// Group (no value)
	tagTypeParam		// Parameter (with a value)
};

enum multiLineGroup
{
	multiLineGroupYes = 0,	// This group can extend across many lines
	multiLineGroupNo		// This group cannot extend across many lines
};

struct elementInfo_t
{
	elementInfo_t() : lineElementId(-1),predefinedValue(""),multiLine(multiLineGroupNo){}	// Set some defaults
	string name;				// The name of the tag
	int elementId;				// Element id from xyz_elements
	tagType type;				// Tag type (group or element)
	int lineElementId;			// Element id from line from the input file (-1 if not mapped)
	string predefinedValue;		// Predefined (fixed) element value if applicable
	multiLineGroup multiLine;	// If this group includes subgroups of the same type
	vector<int> subElements;	// Subelements (valid for groups only, of course)
};

// Define all elements
typedef map<int,elementInfo_t> elementInfoMap_t;	// first is enum from xyz_elements definition

//////////////////////////////////////////////////////////
// Specific init functions - do one for your scheme
//////////////////////////////////////////////////////////
// We statically define the XML tree specific for this type of map
static elementInfoMap_t init_Doh_KDVP_map()
{
	elementInfoMap_t map;
	elementInfo_t info;

	// Doh_KDVP_element_KDVPItem
	info = elementInfo_t();
	info.name = "KDVPItem";
	info.elementId = Doh_KDVP_element_KDVPItem;
	info.type = tagTypeGroup;
	info.multiLine = multiLineGroupYes;
	info.subElements.push_back(Doh_KDVP_element_InventoryListType);
	info.subElements.push_back(Doh_KDVP_element_Name);
	info.subElements.push_back(Doh_KDVP_element_HasForeignTax);
	info.subElements.push_back(Doh_KDVP_element_HasLossTransfer);
	info.subElements.push_back(Doh_KDVP_element_ForeignTransfer);
	info.subElements.push_back(Doh_KDVP_element_TaxDecreaseConformance);
	info.subElements.push_back(Doh_KDVP_element_Securities);
	info.subElements.push_back(Doh_KDVP_element_SecuritiesShort);
	map[Doh_KDVP_element_KDVPItem] = info;

	// Doh_KDVP_element_InventoryListType
	info = elementInfo_t();
	info.name = "InventoryListType";
	info.elementId = Doh_KDVP_element_InventoryListType;
	info.type = tagTypeParam;
	map[Doh_KDVP_element_InventoryListType] = info;

	// Doh_KDVP_element_Name
	info = elementInfo_t();
	info.name = "Name";
	info.elementId = Doh_KDVP_element_Name;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_ticker;
	map[Doh_KDVP_element_Name] = info;

	// Doh_KDVP_element_HasForeignTax
	info = elementInfo_t();
	info.name = "HasForeignTax";
	info.elementId = Doh_KDVP_element_HasForeignTax;
	info.type = tagTypeParam;
	info.predefinedValue = "false";
	map[Doh_KDVP_element_HasForeignTax] = info;

	// Doh_KDVP_element_HasLossTransfer
	info = elementInfo_t();
	info.name = "HasLossTransfer";
	info.elementId = Doh_KDVP_element_HasLossTransfer;
	info.type = tagTypeParam;
	info.predefinedValue = "false";
	map[Doh_KDVP_element_HasLossTransfer] = info;

	// Doh_KDVP_element_ForeignTransfer
	info = elementInfo_t();
	info.name = "ForeignTransfer";
	info.elementId = Doh_KDVP_element_ForeignTransfer;
	info.type = tagTypeParam;
	info.predefinedValue = "false";
	map[Doh_KDVP_element_ForeignTransfer] = info;

	// Doh_KDVP_element_TaxDecreaseConformance
	info = elementInfo_t();
	info.name = "TaxDecreaseConformance";
	info.elementId = Doh_KDVP_element_TaxDecreaseConformance;
	info.type = tagTypeParam;
	info.predefinedValue = "false";
	map[Doh_KDVP_element_TaxDecreaseConformance] = info;

	// Doh_KDVP_element_Securities
	info = elementInfo_t();
	info.name = "Securities";
	info.elementId = Doh_KDVP_element_Securities;
	info.type = tagTypeGroup;
	info.multiLine = multiLineGroupYes;
	info.subElements.push_back(Doh_KDVP_element_Code);
	info.subElements.push_back(Doh_KDVP_element_IsFond);
	info.subElements.push_back(Doh_KDVP_element_Row);
	map[Doh_KDVP_element_Securities] = info;

	// Doh_KDVP_element_SecuritiesShort
	info = elementInfo_t();
	info.name = "SecuritiesShort";
	info.elementId = Doh_KDVP_element_SecuritiesShort;
	info.type = tagTypeGroup;
	info.multiLine = multiLineGroupYes;
	info.subElements.push_back(Doh_KDVP_element_Code);
	info.subElements.push_back(Doh_KDVP_element_IsFond);
	info.subElements.push_back(Doh_KDVP_element_Row);
	map[Doh_KDVP_element_SecuritiesShort] = info;

	// Doh_KDVP_element_Code
	info = elementInfo_t();
	info.name = "Code";
	info.elementId = Doh_KDVP_element_Code;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_ticker;
	map[Doh_KDVP_element_Code] = info;

	// Doh_KDVP_element_IsFond
	info = elementInfo_t();
	info.name = "IsFond";
	info.elementId = Doh_KDVP_element_IsFond;
	info.type = tagTypeParam;
	info.predefinedValue = "false";
	map[Doh_KDVP_element_IsFond] = info;

	// Doh_KDVP_element_Row
	info = elementInfo_t();
	info.name = "Row";
	info.elementId = Doh_KDVP_element_Row;
	info.type = tagTypeGroup;
	info.subElements.push_back(Doh_KDVP_element_ID);
	info.subElements.push_back(Doh_KDVP_element_Purchase);
	info.subElements.push_back(Doh_KDVP_element_Sale);
	info.subElements.push_back(Doh_KDVP_element_F8);
	map[Doh_KDVP_element_Row] = info;

	// Doh_KDVP_element_ID
	info = elementInfo_t();
	info.name = "ID";
	info.elementId = Doh_KDVP_element_ID;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_trans_id;
	map[Doh_KDVP_element_ID] = info;

	// Doh_KDVP_element_Purchase
	info = elementInfo_t();
	info.name = "Purchase";
	info.elementId = Doh_KDVP_element_Purchase;
	info.type = tagTypeGroup;
	info.subElements.push_back(Doh_KDVP_element_F1);
	info.subElements.push_back(Doh_KDVP_element_F2);
	info.subElements.push_back(Doh_KDVP_element_F3);
	info.subElements.push_back(Doh_KDVP_element_F4);
	map[Doh_KDVP_element_Purchase] = info;

	// Doh_KDVP_element_Sale
	info = elementInfo_t();
	info.name = "Sale";
	info.elementId = Doh_KDVP_element_Sale;
	info.type = tagTypeGroup;
	info.subElements.push_back(Doh_KDVP_element_F6);
	info.subElements.push_back(Doh_KDVP_element_F7);
	info.subElements.push_back(Doh_KDVP_element_F9);
	info.subElements.push_back(Doh_KDVP_element_F10);
	map[Doh_KDVP_element_Sale] = info;

	// Doh_KDVP_element_F1
	info = elementInfo_t();
	info.name = "F1";
	info.elementId = Doh_KDVP_element_F1;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_date;
	map[Doh_KDVP_element_F1] = info;

	// Doh_KDVP_element_F2
	info = elementInfo_t();
	info.name = "F2";
	info.elementId = Doh_KDVP_element_F2;
	info.type = tagTypeParam;
	info.predefinedValue = "";
	map[Doh_KDVP_element_F2] = info;

	// Doh_KDVP_element_F3
	info = elementInfo_t();
	info.name = "F3";
	info.elementId = Doh_KDVP_element_F3;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_shares;
	map[Doh_KDVP_element_F3] = info;

	// Doh_KDVP_element_F4
	info = elementInfo_t();
	info.name = "F4";
	info.elementId = Doh_KDVP_element_F4;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_price;
	map[Doh_KDVP_element_F4] = info;

	// Doh_KDVP_element_F6
	info = elementInfo_t();
	info.name = "F6";
	info.elementId = Doh_KDVP_element_F6;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_date;
	map[Doh_KDVP_element_F6] = info;

	// Doh_KDVP_element_F7
	info = elementInfo_t();
	info.name = "F7";
	info.elementId = Doh_KDVP_element_F7;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_shares;
	map[Doh_KDVP_element_F7] = info;

	// Doh_KDVP_element_F8
	info = elementInfo_t();
	info.name = "F8";
	info.elementId = Doh_KDVP_element_F8;
	info.type = tagTypeParam;
	map[Doh_KDVP_element_F8] = info;

	// Doh_KDVP_element_F9
	info = elementInfo_t();
	info.name = "F9";
	info.elementId = Doh_KDVP_element_F9;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_price;
	map[Doh_KDVP_element_F9] = info;

	// Doh_KDVP_element_F10
	info = elementInfo_t();
	info.name = "F10";
	info.elementId = Doh_KDVP_element_F10;
	info.type = tagTypeParam;
	info.lineElementId = Doh_KDVP_line_values_loss_valid;
	map[Doh_KDVP_element_F10] = info;

	return map;
}

//////////////////////////////////////////////////////////
// Specific scheme data - set these parameters for your scheme
//////////////////////////////////////////////////////////
// Item identifier
static const unsigned item_identifier = Doh_KDVP_line_values_trans_id;

// Initialize element info map
// This is the basic map we work with when building XML structure
static const elementInfoMap_t elementInfoMap = init_Doh_KDVP_map();

// Expected number of line values
static const unsigned lineValuesNum = Doh_KDVP_line_values_last;

// Vector that stores line values
// Mind that order of these values in vector is important
static vector<string> lineValues(lineValuesNum);

// Root element - algorithm starts building element tree from here
static const int rootElement = Doh_KDVP_element_KDVPItem;

////////////////////////////////////////////////////////
// Base input, output and currency convertion file names
////////////////////////////////////////////////////////

static const string baseFileName = "base.xml";
static const string inputFileName = "input.csv";
static const string outputFileName = "output.xml";
static const string currConvFileName = "conversion.xml";


/////////////////////////////////////////////////////
//	Data for string parsing and building XML
//	These need to be changed for any other XML scheme
/////////////////////////////////////////////////////

static const char delimiter = ';';

// Depth at which to start adding our tags
static const string startDepth = "			";
static string depth = startDepth;

// String in file that defines location where we should insert our content
// </KDVP> tag concludes header after which items should follow
static const string insertContentAfter = "</KDVP>";

// Base, input, output and currencty convertion files can be global
// because all functions work with them
static ifstream baseFile;
static ifstream inputFile;
static ofstream outputFile;
static ifstream currConvFile;

// If we want to include currency converting or not
static bool useCurrConversion = false;

#define DEPTH_PLUS \
		depth.append("	");

#define DEPTH_MINUS \
		depth.erase(depth.end()-1, depth.end());

// Depth macros
#define DEPTH0(f) \
		f << "\r\n" << startDepth;
#define DEPTH1(f) \
		f << "\r\n" << startDepth << "	";
#define DEPTH2(f) \
		f << "\r\n" << startDepth << "		";

// Macros for XML building
#define ADD_ELEMENT(f, tag, element) \
{\
		f << /*"\r\n" << depth <<*/ "<" << tag << ">"; \
		f << element; \
		f << "</" << tag << ">"; \
}

#define ADD_GROUP(f, tag) \
		f << /*"\r\n" << depth <<*/ "<" << tag << ">";

#define END_GROUP(f, tag) \
		f << /*"\r\n" << depth <<*/ "</" << tag << ">";

// Remaining shares of the current item
static int remainingShares = 0;

// Local conversion storage
static map<string,string> conversionMap;	// <date,rate>

/////////////////////////////////////////////////////
// Static functions that perform different operations
/////////////////////////////////////////////////////

// Convert conversion file to local storage
static bool conversionFileToLocalStorage(const string fileName)
{
	string line, dateStr, currStr;
	ifstream inputFile;
	const string tagTecajnica = "tecajnica datum";	// Defines new date
	const string tagTecaj = "USD";					// Defines USD rate
	vector<pair<string,string> > vec;				// Vector of pairs <date,rate>

	// Open file
	inputFile.open(fileName.c_str(), ifstream::in);
	
	if (!inputFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open input conversion file!" << endl;
		return false;
	}
	
	// The main loop -
	// Read input file line by line and store date and value in local storage
	while(getline(inputFile, line))
	{
		if (line.find(tagTecajnica) != std::string::npos)
		{
			// New date - just store it
			dateStr = line.substr(line.find("\"")+1, 10);
		}
		else if (line.find(tagTecaj) != std::string::npos)
		{
			// New rate
			currStr = line.substr(line.find(">")+1, 6);
			
			// Push current date/rate combination into vector
			vec.push_back(make_pair(dateStr,currStr));

		}
	}

	// Close file
	inputFile.close();
	string lastValidDate, lastValidRate;
	unsigned thisYear, thisMonth, thisDay;
	unsigned lastYear, lastMonth, lastDay;
	stringstream ss, ss1;

	// We'll add missing days (weekends, holidays, etc.) to avoid date misses
	// Mind that we assume that values are pushed back in ascending order of dates from XML!
	for (vector<pair<string,string> >::iterator it = vec.begin(); it != vec.end(); it++)
	{
		if (lastValidDate.size() > 0 && lastValidRate.size() > 0)
		{
			// If there are gaps in between last valid and current date,
			// we have to fill them with last valid date
			
			// This...
			ss.str("");
			ss.clear();
			ss << it->first.substr(0, 4);
			ss >> thisYear;
			ss.str("");
			ss.clear();
			ss << it->first.substr(5, 2);
			ss >> thisMonth;
			ss.str("");
			ss.clear();
			ss << it->first.substr(8, 2);
			ss >> thisDay;
			
			// Last valid...
			ss.str("");
			ss.clear();
			ss << lastValidDate.substr(0, 4);
			ss >> lastYear;
			ss.str("");
			ss.clear();
			ss << lastValidDate.substr(5, 2);
			ss >> lastMonth;
			ss.str("");
			ss.clear();
			ss << lastValidDate.substr(8, 2);
			ss >> lastDay;
			
			if (thisYear == lastYear)
			{
				// Same year
				if (thisMonth == lastMonth)
				{
					// Same month
					if (thisDay > lastDay + 1)
					{
						// Gaps!
						for (int i = lastDay + 1; i < thisDay; i++)
						{
							ss1.str("");
							ss1.clear();
							// Compose date and push it to map with last valid rate
							ss1 << thisYear << "-" << setw(2) << setfill('0') << thisMonth << "-" << setw(2) << setfill('0') << i;
							
							// Do some error checking
							if (conversionMap.find(ss1.str()) != conversionMap.end())
							{
								cout << __FUNCTION__ << ":" << __LINE__ << ": "
									<< "Date already exists in conversion map: " << ss1.str() << endl;
					 
								return false;
							}
							else
								conversionMap[ss1.str()] = lastValidRate;
						}
					}
				}
				else if (thisMonth == lastMonth + 1)
				{
					// New month in same year
					if (lastDay < 31)
					{
						// Gaps at the end of last month
						for (int i = lastDay + 1; i <= 31; i++)
						{
							ss1.str("");
							ss1.clear();
							// Compose date and push it to map with last valid rate
							ss1 << thisYear << "-" << setw(2) << setfill('0') << lastMonth << "-" << setw(2) << setfill('0') << i;
							
							if (conversionMap.find(ss1.str()) != conversionMap.end())
							{
								cout << __FUNCTION__ << ":" << __LINE__ << ": "
									<< "Date already exists in conversion map: " << ss1.str() << endl;
					 
								return false;
							}
							else
								conversionMap[ss1.str()] = lastValidRate;
						}
					}
					
					if (thisDay > 1)
					{
						// Gaps at the beginning of new month
						for (int i = 1; i < thisDay; i++)
						{
							ss1.str("");
							ss1.clear();
							// Compose date and push it to map with last valid rate
							ss1 << thisYear << "-" << setw(2) << setfill('0') << thisMonth << "-" << setw(2) << setfill('0') << i;
							
							if (conversionMap.find(ss1.str()) != conversionMap.end())
							{
								cout << __FUNCTION__ << ":" << __LINE__ << ": "
									<< "Date already exists in conversion map: " << ss1.str() << endl;
					 
								return false;
							}
							else
								conversionMap[ss1.str()] = lastValidRate;
						}
					}
				}
				else
				{
					cout << __FUNCTION__ << ":" << __LINE__ << ": "
						<< "Skipping month??: " << lastValidDate << ">" << it->first << endl;
					 
					return false;
				}
			}
			else if (thisYear == lastYear + 1)
			{
				// New year
				if (thisMonth != 1 || lastMonth != 12)
				{
					cout << __FUNCTION__ << ":" << __LINE__ << ": "
						<< "Skipping month of new year??: " << lastValidDate << ">" << it->first << endl;
					 
					return false;
				}
				
				// New month in new year
				if (lastDay < 31)
				{
					// Gaps at the end of last month
					for (int i = lastDay + 1; i <= 31; i++)
					{
						ss1.str("");
						ss1.clear();
						// Compose date and push it to map with last valid rate
						ss1 << lastYear << "-" << setw(2) << setfill('0') << lastMonth << "-" << setw(2) << setfill('0') << i;
						
						if (conversionMap.find(ss1.str()) != conversionMap.end())
						{
							cout << __FUNCTION__ << ":" << __LINE__ << ": "
								<< "Date already exists in conversion map: " << ss1.str() << endl;
				 
							return false;
						}
						else
							conversionMap[ss1.str()] = lastValidRate;
					}
				}
					
				if (thisDay > 1)
				{
					// Gaps at the beginning of new month of new year
					for (int i = 1; i < thisDay; i++)
					{
						ss1.str("");
						ss1.clear();
						// Compose date and push it to map with last valid rate
						ss1 << thisYear << "-" << setw(2) << setfill('0') << thisMonth << "-" << setw(2) << setfill('0') << i;
						
						if (conversionMap.find(ss1.str()) != conversionMap.end())
						{
							cout << __FUNCTION__ << ":" << __LINE__ << ": "
								<< "Date already exists in conversion map: " << ss1.str() << endl;
				 
							return false;
						}
						else
							conversionMap[ss1.str()] = lastValidRate;
					}
				}
			}
			else
			{
				cout << __FUNCTION__ << ":" << __LINE__ << ": "
					 << "Skipping year??: " << lastValidDate << ">" << it->first << endl;
					 
				return false;
			}
		}
		
		// Now add current date to map and store last valid date
		if (conversionMap.find(it->first) != conversionMap.end())
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Date already exists in conversion map: " << it->first << endl;
				
			for (map<string,string>::iterator it = conversionMap.begin(); it != conversionMap.end(); it++)
			cout << it->first << "," << it->second << endl;
		
			return false;
		}
		else
			conversionMap[it->first] = it->second;
		
		
		lastValidDate = it->first;
		lastValidRate = it->second;
	}

	return true;
}

// Return if we are processing an existing item
static bool ifExistingItem(const elementInfo_t& element)
{
	if (lineValues[Doh_KDVP_line_values_trans_id] != "0")
		return true;
	else
		return false;
}

// Set beginning and ending string from base file
static void setBaseStr(string& beginStr, string& endStr)
{
	string line;
	string baseFileString;
	stringstream ss;
	size_t pos;

	while(getline(baseFile, line))
		ss << line;

	baseFileString = ss.str();

	pos = baseFileString.find(insertContentAfter);
	pos += insertContentAfter.size();

	beginStr = baseFileString.substr(0, pos);
	endStr = baseFileString.substr(pos);
}

// Set line values in a structure which we will use to actually build elements
static int setLineValues(string& str)
{
	int prevDelim = -1, currPos = -1;
	string element;
	unsigned elementId = 0;

	// On some systems line includes carriage return at the end
	// which must be removed
	if (str.length() > 0)
	{
		string::iterator it = str.end() - 1;
		if (*it == '\r')
			str.erase(it);
	}

	// Clear lineValues vector
	lineValues.clear();

	// Get elements from line string
	stringstream ss(str);
	while(getline(ss, element, delimiter))
		lineValues.push_back(element);

	return 0;
}

// Get if element should be shown
static bool doShowElement(const elementInfo_t& element)
{
	// Some parameters are shown only under certain conditions
	if (element.elementId == Doh_KDVP_element_Purchase)
	{
		// Show if line element Transaction Type is "Buy" or "BuyCover"
		if (lineValues[Doh_KDVP_line_values_trans_type] != "Buy" &&
			lineValues[Doh_KDVP_line_values_trans_type] != "BuyCover")
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_Sale)
	{
		// Show if line element Transaction Type is "Sell" or "SellShort"
		if (lineValues[Doh_KDVP_line_values_trans_type] != "Sell" &&
			lineValues[Doh_KDVP_line_values_trans_type] != "SellShort")
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_Securities)
	{
		// Show if line element Transaction Type is "Buy" or "Sell"
		if (lineValues[Doh_KDVP_line_values_trans_type] != "Buy" &&
			lineValues[Doh_KDVP_line_values_trans_type] != "Sell")
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_SecuritiesShort)
	{
		// Show if line element Transaction Type is "SellShort" or "BuyCover"
		if (lineValues[Doh_KDVP_line_values_trans_type] != "SellShort" &&
			lineValues[Doh_KDVP_line_values_trans_type] != "BuyCover")
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_F10)	// Loss valid
	{
		// Show if line element Transaction Type is Sell"
		if (lineValues[Doh_KDVP_line_values_trans_type] != "Sell")
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_InventoryListType ||
			 element.elementId == Doh_KDVP_element_Name ||
			 element.elementId == Doh_KDVP_element_HasForeignTax ||
			 element.elementId == Doh_KDVP_element_HasLossTransfer ||
			 element.elementId == Doh_KDVP_element_ForeignTransfer ||
			 element.elementId == Doh_KDVP_element_TaxDecreaseConformance ||
			 element.elementId == Doh_KDVP_element_Securities ||
			 element.elementId == Doh_KDVP_element_SecuritiesShort ||
			 element.elementId == Doh_KDVP_element_Code ||
			 element.elementId == Doh_KDVP_element_IsFond)
	{
		// If the same item is being processed, don't show
		if (ifExistingItem(element))
			return false;
	}

	return true;
}

// Get converted price parameter value
static int getConvertionRate(const string& date, string& convertionRate)
{
	map<string,string>::iterator it;
	if ((it = conversionMap.find(date)) == conversionMap.end())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "No conversion rate for date: " << date << endl;
		return 1;
	}
	else
		convertionRate = it->second;

	return 0;
}

// Get converted price parameter value
static int getConvertedPrice(const elementInfo_t& element, string& convertedPrice)
{
	if (element.elementId != Doh_KDVP_element_F4 &&
		element.elementId != Doh_KDVP_element_F9)
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
			<< "Invalid elementId: " << element.elementId << endl;

		return 1;
	}

	// Get convertion rate from currConvFile
	string convRateStr;
	if (getConvertionRate(lineValues[Doh_KDVP_line_values_date], convRateStr) != 0)
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
			<< "getConvertionRate failed for date: " << lineValues[Doh_KDVP_line_values_date] << endl;

		return 1;
	}

	// Convert rate using convertion rate
	double convRateDouble, priceDouble, convPriceDouble;
	convRateDouble = atof(convRateStr.c_str());
	priceDouble = atof(lineValues[Doh_KDVP_line_values_price].c_str());
	convPriceDouble = priceDouble/convRateDouble;

	stringstream ss;
	ss << fixed << setprecision(4) << convPriceDouble;
	convertedPrice = ss.str();

	return 0;
}

// Get specific element parameter value
static int getSpecificParamValue(const elementInfo_t& element, string& val)
{
	if (element.elementId == Doh_KDVP_element_InventoryListType)
	{
		// InventoryListType can be "PLVP" or "PLVPSHORT" depending on
		// line value Transaction Type
		if (lineValues[Doh_KDVP_line_values_trans_type] == "Sell" ||
			lineValues[Doh_KDVP_line_values_trans_type] == "SellShort")
			val = "PLVPSHORT";
		else
			val = "PLVP";
	}
	else if (element.elementId == Doh_KDVP_element_F1)
	{
		// F2 is "B" for long purchase and "A" for short cover purchase
		if (lineValues[Doh_KDVP_line_values_trans_type] == "Buy")
		{
			// Long purchase
			val = "B";
		}
		else if (lineValues[Doh_KDVP_line_values_trans_type] == "BuyCover")
		{
			// Short cover purchase
			val = "A";
		}
	}
	else if (element.elementId == Doh_KDVP_element_F2)
	{
		// F2 is "B" for long purchase and "A" for short cover purchase
		if (lineValues[Doh_KDVP_line_values_trans_type] == "Buy")
		{
			// Long purchase
			val = "B";
		}
		else if (lineValues[Doh_KDVP_line_values_trans_type] == "BuyCover")
		{
			// Short cover purchase
			val = "A";
		}
	}
	else if (element.elementId == Doh_KDVP_element_F8)
	{
		// Remaining shares - calculate from existing shares and current change
		int valInt = 0;
		unsigned currVal = 0;
		stringstream currValSs;
		stringstream valSs;

		// If this is new item, reset remainingShares
		if (!ifExistingItem(element))
			remainingShares = 0;

		// Get current shares value
		currValSs << lineValues[Doh_KDVP_line_values_shares];
		currValSs >> currVal;

		// Calculate remaining shares according to transaction type
		if (lineValues[Doh_KDVP_line_values_trans_type] == "Buy" ||
			lineValues[Doh_KDVP_line_values_trans_type] == "BuyCover")
		{
			valInt = remainingShares + currVal;
		}
		if (lineValues[Doh_KDVP_line_values_trans_type] == "Sell" ||
			lineValues[Doh_KDVP_line_values_trans_type] == "SellShort")
		{
			valInt = remainingShares - currVal;
		}

		// Set output value
		valSs << valInt;
		if (valInt != 0)
			valSs << ".0000";	// Not sure if this is really necessary, but it doesn't hurt

		val = valSs.str();

		// Store remaining shares
		remainingShares = valInt;
	}

	return 0;
}

// Get element parameter value
static int getElementParamValue(const elementInfo_t& element, string& val)
{
	// First of all, this should actually be a parameter
	if (element.type != tagTypeParam)
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Element is not a parameter: " << element.name << endl;
		return 1;
	}

	// Get value
	if (element.predefinedValue.compare(""))
	{
		// If there is a predefined value, we return it
		val = element.predefinedValue;
	}
	else if (element.lineElementId != -1)
	{
		// If lineElementId is not -1, this parameter is mapped from lineValues vector
		if (useCurrConversion && element.lineElementId == Doh_KDVP_line_values_price)
		{
			// Convert price value
			string convertVal;
			if (getConvertedPrice(element, convertVal) != 0)
			{
				cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "getConvertedValue failed for " << element.name << endl;

				return 1;
			}

			val = convertVal;
		}
		else
			val = lineValues.at(element.lineElementId);	// No conversion
	}
	else
	{
		// This is some specific parameter
		if (getSpecificParamValue(element, val) != 0)
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot read value for element " << element.name << endl;

			return 1;
		}
	}

	return 0;
}

static unsigned numLines = 0;			// Number of lines in file
static unsigned lineNum = 0;			// Current line number

// Map of groups that should be closed on new item
struct closeGroupsData_t
{
	string groupName;
	unsigned depth;
};
static closeGroupsData_t closeGroupsData;
static vector<closeGroupsData_t> closeGroups;

// Return if new multi line group should be started
static bool ifStartNewMultiLineGroup(const elementInfo_t& element, unsigned& depth)
{
	bool ret = true;

	if (elementInfoMap.at(element.elementId).type != tagTypeGroup)
		return ret;

	// Set if group should be started in present context
	if (element.elementId == Doh_KDVP_element_KDVPItem)
	{
		if (ifExistingItem(element))
		{
			ret = false;
			depth = 0;
		}
	}
	else if (element.elementId == Doh_KDVP_element_Securities)
	{
		if (ifExistingItem(element) &&
		    (lineValues[Doh_KDVP_line_values_trans_type] == "Buy" ||
		    lineValues[Doh_KDVP_line_values_trans_type] == "Sell"))
		{
			ret = false;
			depth = 1;
		}
	}
	else if (element.elementId == Doh_KDVP_element_SecuritiesShort)
	{
		if (ifExistingItem(element) &&
			(lineValues[Doh_KDVP_line_values_trans_type] == "BuyCover" ||
			lineValues[Doh_KDVP_line_values_trans_type] == "SellShort"))
		{
			ret = false;
			depth = 1;
		}
	}

	return ret;
}

// Return if opened groups should be closed
static bool ifCloseOpenGroups(const elementInfo_t& element)
{
	if (element.elementId == Doh_KDVP_element_KDVPItem &&
		!ifExistingItem(element))
		return true;

	return false;
}

// Core function that processes elements of the XML tree
// It can be called recursively while processing current element
static int processElement(const elementInfo_t& element)
{
	string elementValue;

	// First check if previous opened supergroups should be closed
	if (ifCloseOpenGroups(element))
	{
		for (int i = closeGroups.size() - 1; i >=0; i--)
		{
			for (int j = 0; j < closeGroups.at(i).depth; j++)
				DEPTH_PLUS

			END_GROUP(outputFile, closeGroups.at(i).groupName)

			for (int j = 0; j < closeGroups.at(i).depth; j++)
				DEPTH_MINUS
		}
		closeGroups.clear();
	}

	// Continue if this element must be shown in present context
	if (!doShowElement(element))
		return 0;

	// Basic rules for building tags:
	//		- change depth using DEPTH_PLUS or DEPTH_MINUS (otherwise it stays as it is)
	//		- use ADD_GROUP and END_GROUP to start or end a new group
	//		- use ADD_ELEMENT to add new element to the group

	// name should never be empty
	if (!elementInfoMap.at(element.elementId).name.compare(""))
	{
		cout << "Empty name for element " << element.elementId << endl;
		return 1;
	}

	if (elementInfoMap.at(element.elementId).type == tagTypeGroup)
	{
		unsigned endDepth;

		// If this is a multiline group, we must check if we must start
		// a new one or if we are just adding a new line inside this group
		if (elementInfoMap.at(element.elementId).multiLine == multiLineGroupYes)
		{
			if (ifStartNewMultiLineGroup(element, endDepth))
				ADD_GROUP(outputFile, elementInfoMap.at(element.elementId).name)
			else
			{
				// We are continuing an existing group, which will have to be
				// closed some time in the future, so store its data for closure
				bool exists = false;
				for (int i = 0; i < closeGroups.size(); i++)
				{
					if (closeGroups[i].groupName == element.name)
					{
						exists = true;
						break;
					}
				}

				// Add each group for closure only once
				if (!exists)
				{
					closeGroupsData.groupName = element.name;
					closeGroupsData.depth = endDepth;
					closeGroups.push_back(closeGroupsData);
				}
			}
		}
		else	// Groups without subgroups are always created
			ADD_GROUP(outputFile, elementInfoMap.at(element.elementId).name)

		// Go through all sub-elements of this group and process them recursively
		for (vector<int>::const_iterator it =
				elementInfoMap.at(element.elementId).subElements.begin();
				it != elementInfoMap.at(element.elementId).subElements.end(); it++)
		{
			if (elementInfoMap.find(*it) == elementInfoMap.end())
			{
				cout << __FUNCTION__ << ":" << __LINE__ << ": " << ": "
					 << "Cannot find element " << *it << endl;
				return 1;
			}

			// One depth deeper
			DEPTH_PLUS
			if (processElement(elementInfoMap.at(*it)) != 0)
			{
				cout << __FUNCTION__ << ":" << __LINE__ << ": "
						<< "Error processing element " << elementInfoMap.at(*it).name << endl;
				return 1;
			}
			DEPTH_MINUS
		}

		// End here only groups which are not multilines
		if (elementInfoMap.at(element.elementId).multiLine != multiLineGroupYes)
			END_GROUP(outputFile, elementInfoMap.at(element.elementId).name)
		else if (lineNum == numLines)	// Last line, close final groups
			END_GROUP(outputFile, elementInfoMap.at(element.elementId).name)
	}
	else if (elementInfoMap.at(element.elementId).type == tagTypeParam)
	{
		// Parameter element - tag with value
		if (getElementParamValue(elementInfoMap.at(element.elementId), elementValue) != 0)
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Cannot read value for element " << element.elementId << endl;
			return 1;
		}

		// Empty parameters are never displayed
		if (elementValue != "")
			ADD_ELEMENT(outputFile, elementInfoMap.at(element.elementId).name, elementValue)
	}
	else
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Unknown tag type " << elementInfoMap.at(element.elementId).type << endl;
		return 1;
	}

	return 0;
}

// Usage: xyz.exe [convert]
int main(int argc, char* argv[])
{
	// Strings from base file that should be inserted before and after our content
	string beforeBaseStr;
	string afterBaseStr;

	if (argc >= 2 && !strcmp(argv[1], "convert"))
	{
		// Transform currency conversion file to local storage
		if (!conversionFileToLocalStorage(currConvFileName))
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Error converting currency file to local storage!" << endl;
				
			return 1;
		}
		cout << "Using currency conversion" << endl;
		useCurrConversion = true;
	}
	else
		cout << "No currency conversion" << endl;

	// Open all files
	baseFile.open(baseFileName.c_str(), ifstream::in);
	inputFile.open(inputFileName.c_str(), ifstream::in);
	outputFile.open(outputFileName.c_str(), ofstream::out|ofstream::binary);

	if (!baseFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open base file!" << endl;
		return 1;
	}
	else if (!inputFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open input file!" << endl;
		return 1;
	}
	else if (!outputFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open output file!" << endl;
		return 1;
	}

	// Set before and after strings
	setBaseStr(beforeBaseStr, afterBaseStr);

	// Copy "before" string into file
	outputFile << beforeBaseStr;

	string line;
	// Read file once just to set numLines
	while(getline(inputFile, line))
		numLines++;

	// Set stream pointer to beginning
	inputFile.clear();
	inputFile.seekg(0);

	// The main loop -
	// Read input file line by line and insert values in xml
	while(getline(inputFile, line))
	{
		lineNum++;
		if (setLineValues(line) != 0)
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Error setting line values at line " << lineNum << endl;
			return 1;
		}

		// Start processing from rootElement
		if (elementInfoMap.find(rootElement) == elementInfoMap.end())
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Cannot find root element " << rootElement << endl;
			return 1;
		}

		if (processElement(elementInfoMap.at(rootElement)) != 0)
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Error processing root element " << rootElement << endl;
			return 1;
		}
	}

	// Copy "after" string into file
	outputFile << afterBaseStr;

	// Close all files
	baseFile.close();
	inputFile.close();
	outputFile.close();

	return 0;
}

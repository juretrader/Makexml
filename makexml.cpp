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
#include <ctime>

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
static const unsigned currLineValuesNum = Doh_KDVP_line_values_last;

// Root element - algorithm starts building element tree from here
static const int rootElement = Doh_KDVP_element_KDVPItem;

// Name transaction types
enum transactionType
{
	transactionTypeBuy = 0,
	transactionTypeSell,
	transactionTypeSellShort,
	transactionTypeBuyCover,
};

static map<transactionType,string> init_transaction_names()
{
	map<transactionType,string> tmpMap;
	tmpMap[transactionTypeBuy] = "Buy";
	tmpMap[transactionTypeSell] = "Sell";
	tmpMap[transactionTypeSellShort] = "SellShort";
	tmpMap[transactionTypeBuyCover] = "BuyCover";
	return tmpMap;
};

static map<transactionType,string> transactionTypeNames = init_transaction_names();

////////////////////////////////////////////////////////////////
// Base input, output and currency convertion default file names
////////////////////////////////////////////////////////////////

static string baseFileName = "base.xml";
static string inputFileName = "input.csv";
static string outputFileName = "output.xml";
static string currConvFileName = "conversion.xml";
static string tagTecaj = "USD";

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

// Vector that stores current line values
// Mind that order of these values in vector is important
typedef vector<string> lineValues_t;
static lineValues_t currLineValues(currLineValuesNum);

// A map of transactions used as the basic input for XML
// Trades are sorted in the following way:
//   - ticker
//     - long trades
//       - transaction_id -> values
//     - short trades
//       - transaction_id -> values

enum tradeDirection
{
	tradeLong = 0,
	tradeShort = 1
};

typedef map<string,lineValues_t> transactionIdMap_t;					// <transaction_id,line_values>
typedef map<tradeDirection,transactionIdMap_t> tradeDirectionMap_t;		// <tradeDirection, ...>
typedef map<string,tradeDirectionMap_t> transactionMap_t;				// <ticker, ... >
static transactionMap_t transactionMap;

// Local conversion storage
static map<string,string> conversionMap;	// <date,rate>

// A variable denoting if the last element is being processed
static bool lastElement = false;

// Already washed shares for this ticker - needed for wash sale calculation
static unsigned alreadyWashed = 0;

/////////////////////////////////////////////////////
// Static functions that perform different operations
/////////////////////////////////////////////////////

// Convert conversion file to local storage
static bool conversionFileToLocalStorage(const string fileName)
{
	string line, dateStr, currStr;
	ifstream inputFile;
	const string tagTecajnica = "tecajnica datum";	// Defines new date
	vector<pair<string,string> > vec;				// Vector of pairs <date,rate>

	// Open file
	inputFile.open(fileName.c_str(), ifstream::in);
	
	if (!inputFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open input conversion file!" << endl;
		return false;
	}
	
	// Read input file line by line and store date and value in local storage
	while(getline(inputFile, line))
	{
		if (line.find(tagTecajnica) != std::string::npos)
		{
			// New date - just store it
			dateStr = line.substr(line.find_first_of("\"")+1, line.find_last_of("\"")-line.find_first_of("\"")-1);
		}
		else if (line.find(tagTecaj) != std::string::npos)
		{
			// New rate
			currStr = line.substr(line.find_first_of(">")+1, line.find_last_of("<")-line.find_first_of(">")-1);
			
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
	if (currLineValues[Doh_KDVP_line_values_trans_id] != "0")
		return true;
	else
		return false;
}

// Set beginning and ending string from base file
static void setBaseStr(string& beginStr, string& endStr, ifstream& baseFile)
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

// Set line values in a vector
static int setLineValues(string& str, lineValues_t& lineVec)
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

	// Get elements from line string
	stringstream ss(str);
	while(getline(ss, element, delimiter))
		lineVec.push_back(element);

	return 0;
}

// Convert input file to local storage
static bool inputFileToLocalStorage(const string fileName)
{
	ifstream inputFile;
	string line;
	lineValues_t currLineVec;

	// Open input file
	inputFile.open(fileName.c_str(), ifstream::in);

	if (!inputFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open input file!" << endl;
		return 1;
	}

	// Set stream pointer to beginning
	inputFile.clear();
	inputFile.seekg(0);

	// Read input file line by line and store values
	unsigned lineNum = 0;

	while(getline(inputFile, line))
	{
		lineNum++;
		currLineVec.clear();
		if (setLineValues(line, currLineVec) != 0)
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Error setting line values at line " << lineNum << endl;
			return 1;
		}

		// Get trade direction
		tradeDirection direction;
		if (currLineVec[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuy] ||
			currLineVec[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSell])
		{
			// Long
			direction = tradeLong;
		}
		else if (currLineVec[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSellShort] ||
				 currLineVec[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuyCover])
		{
			// Short
			direction = tradeShort;
		}
		else
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Unknown transaction type '" <<  currLineVec[Doh_KDVP_line_values_trans_type]
					<< "' at line: " << lineNum << endl;

			return 1;
		}

		// Add line into map at the correct place (long or short trade for this ticker)
		transactionMap_t::iterator it1 = transactionMap.find(currLineVec[Doh_KDVP_line_values_ticker]);
		if (it1 != transactionMap.end())
		{
			// Ticker already exists in map
			// Check if this transaction id already exists for this direction
			transactionIdMap_t::iterator it2 = it1->second[direction].find(currLineVec[Doh_KDVP_line_values_trans_id]);
			if (it2 != it1->second[direction].end())
			{
				cout << __FUNCTION__ << ":" << __LINE__ << ": "
						<< "Transaction id '" << currLineVec[Doh_KDVP_line_values_trans_id]
						<< "' already exists for ticker: " << currLineVec[Doh_KDVP_line_values_ticker]
						<< ", direction: " << (direction == tradeLong ? "Long" : "Short")
						<< ", line: " << lineNum << endl;

				return false;
			}
			else
				it1->second[direction][currLineVec[Doh_KDVP_line_values_trans_id]] = currLineVec;
		}
		else
		{
			// New ticker in map - create both long and short empty maps
			tradeDirectionMap_t tmpTradeDirectionMap;
			tmpTradeDirectionMap[tradeLong];
			tmpTradeDirectionMap[tradeShort];
			transactionMap[currLineVec[Doh_KDVP_line_values_ticker]] = tmpTradeDirectionMap;

			// Insert this trade to correct direction
			tmpTradeDirectionMap.clear();
			transactionIdMap_t tmpTidMap;
			tmpTidMap[currLineVec[Doh_KDVP_line_values_trans_id]] = currLineVec;
			tmpTradeDirectionMap[direction] = tmpTidMap;
			transactionMap[currLineVec[Doh_KDVP_line_values_ticker]] = tmpTradeDirectionMap;
		}
	}

	return true;
}

// Get if element should be shown
static bool doShowElement(const elementInfo_t& element)
{
	// Some parameters are shown only under certain conditions
	if (element.elementId == Doh_KDVP_element_Purchase)
	{
		// Show if line element Transaction Type is transactionTypeNames[transactionTypeBuy] or transactionTypeNames[transactionTypeBuyCover]
		if (currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeBuy] &&
			currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeBuyCover])
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_Sale)
	{
		// Show if line element Transaction Type is transactionTypeNames[transactionTypeSell] or transactionTypeNames[transactionTypeSellShort]
		if (currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeSell] &&
			currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeSellShort])
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_Securities)
	{
		// Show if line element Transaction Type is transactionTypeNames[transactionTypeBuy] or transactionTypeNames[transactionTypeSell]
		if (currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeBuy] &&
			currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeSell])
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_SecuritiesShort)
	{
		// Show if line element Transaction Type is transactionTypeNames[transactionTypeSellShort] or transactionTypeNames[transactionTypeBuyCover]
		if (currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeSellShort] &&
			currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeBuyCover])
			return false;
	}
	else if (element.elementId == Doh_KDVP_element_F10)	// Loss valid
	{
		// Show if line element Transaction Type is Sell"
		if (currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeSell])
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

	// Get convertion rate
	string convRateStr;
	if (getConvertionRate(currLineValues[Doh_KDVP_line_values_date], convRateStr) != 0)
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
			<< "getConvertionRate failed for date: " << currLineValues[Doh_KDVP_line_values_date] << endl;

		return 1;
	}

	// Convert rate using convertion rate
	double convRateDouble, priceDouble, convPriceDouble;
	convRateDouble = atof(convRateStr.c_str());
	priceDouble = atof(currLineValues[Doh_KDVP_line_values_price].c_str());
	convPriceDouble = priceDouble/convRateDouble;

	stringstream ss;
	ss << fixed << setprecision(4) << convPriceDouble;
	convertedPrice = ss.str();

	return 0;
}

// Days between two dates
//	- expects format "yyyy-mm-dd"
//  - expects dateHighStr to be higher date than dateLowStr
static double daysBetweenDates(const string& dateHighStr, const string& dateLowStr)
{
	tm dateHigh = {0};
	tm dateLow = {0};
	unsigned year, month, day;
	stringstream ss;

	// Break down both dates
	ss.str("");
	ss.clear();
	ss << dateHighStr.substr(0, 4);
	ss >> year;
	ss.str("");
	ss.clear();
	ss << dateHighStr.substr(5, 2);
	ss >> month;
	ss.str("");
	ss.clear();
	ss << dateHighStr.substr(8, 2);
	ss >> day;

	dateHigh.tm_mday = day;
	dateHigh.tm_mon = month - 1;
	dateHigh.tm_year = year - 1900;

	ss.str("");
	ss.clear();
	ss << dateLowStr.substr(0, 4);
	ss >> year;
	ss.str("");
	ss.clear();
	ss << dateLowStr.substr(5, 2);
	ss >> month;
	ss.str("");
	ss.clear();
	ss << dateLowStr.substr(8, 2);
	ss >> day;

	dateLow.tm_mday = day;
	dateLow.tm_mon = month - 1;
	dateLow.tm_year = year - 1900;

	time_t timeHigh = mktime(&dateHigh);
	time_t timeLow = mktime(&dateLow);

	double daysDiff = -1;

	if ( timeHigh != (std::time_t)(-1) && timeLow != (std::time_t)(-1) )
		daysDiff = difftime(timeHigh, timeLow) / (60 * 60 * 24);

	return daysDiff;
}

// Get specific element parameter value
static int getSpecificParamValue(const elementInfo_t& element, string& val)
{
	if (element.elementId == Doh_KDVP_element_InventoryListType)
	{
		// InventoryListType can be "PLVP" or "PLVPSHORT" depending on
		// line value Transaction Type
		if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSell] ||
			currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSellShort])
			val = "PLVPSHORT";
		else
			val = "PLVP";
	}
	else if (element.elementId == Doh_KDVP_element_F1)
	{
		// F2 is "B" for long purchase and "A" for short cover purchase
		if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuy])
		{
			// Long purchase
			val = "B";
		}
		else if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuyCover])
		{
			// Short cover purchase
			val = "A";
		}
	}
	else if (element.elementId == Doh_KDVP_element_F2)
	{
		// F2 is "B" for long purchase and "A" for short cover purchase
		if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuy])
		{
			// Long purchase
			val = "B";
		}
		else if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuyCover])
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
		currValSs << currLineValues[Doh_KDVP_line_values_shares];
		currValSs >> currVal;

		// Calculate remaining shares according to transaction type
		if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuy] ||
			currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuyCover])
		{
			valInt = remainingShares + currVal;
		}
		else if (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSell] ||
				currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSellShort])
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
	else if (element.elementId == Doh_KDVP_element_F10)
	{
		// Loss valid
		if (currLineValues[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeSell])
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Invalid trans type for loss valid: " << currLineValues[Doh_KDVP_line_values_trans_type] << endl;

			return 1;
		}

		// Get a copy of all long transactions for this ticker
		transactionIdMap_t tradesMap = transactionMap[currLineValues[Doh_KDVP_line_values_ticker]][tradeLong];

		double daysDiff;
		unsigned currentShares = 0;
		string lastPurchaseDate;
		string lastSaleDate;
		stringstream ss;
		unsigned transThis, transThat;
		unsigned purchasesWithin30DaysBefore = 0;
		unsigned sharesInt;
		unsigned washedShares = 0;			// Value of total shares washed
		bool lastTransactionSale = true;	// Not if last transaction is sale, needed later
		unsigned buysCounter = 0;			// Buys counter
		unsigned sameDaySaleCounter = 0;	// Counter of sales on the same day

		// We have a wash sale under these rules:
		// 	1. We bought additional shares within 30 days after this sale
		//	2. We held some additional shares within 30 days before this sale. This means:
		//		a. We bought more than once within 30 days before
		//		b. We bought only once within 30 days before, but held some shares from before
		//		  (additionalShares is bigger than 0).

		for (transactionIdMap_t::iterator it = tradesMap.begin(); it != tradesMap.end(); it++)
		{
			// Skip the sale we are processing
			if (it->second[Doh_KDVP_line_values_trans_id] == currLineValues[Doh_KDVP_line_values_trans_id])
			{
				lastTransactionSale = true;
				continue;
			}

			// Date before or after this sale?
			ss.str("");
			ss.clear();
			ss << currLineValues[Doh_KDVP_line_values_trans_id];
			ss >> transThis;
			ss.str("");
			ss.clear();
			ss << it->second[Doh_KDVP_line_values_trans_id];
			ss >> transThat;

			if (transThis > transThat)
			{
				// Before or on the same day
				if (it->second[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuy])
				{
					lastTransactionSale = false;
					buysCounter++;

					// Purchase - calculate current shares, washed shares and store last purchase date
					ss.str("");
					ss.clear();
					ss << it->second[Doh_KDVP_line_values_shares];
					ss >> sharesInt;

					daysDiff = daysBetweenDates(currLineValues[Doh_KDVP_line_values_date], it->second[Doh_KDVP_line_values_date]);

					// Was this buy within 30 days and with a newer date than previous and is also not on the day of this sale?
					if (daysDiff > 0 && daysDiff <= 30 && lastPurchaseDate.size() != 0
						&& it->second[Doh_KDVP_line_values_date] != lastPurchaseDate)
					{
 						if (purchasesWithin30DaysBefore > 0)
						{
							// If current count of buys inside 30 days is bigger than 0,
							// we are under wash sale rule 2.a
							washedShares += sharesInt;
						}
 						else if (currentShares > 0 && it->second[Doh_KDVP_line_values_date] != lastPurchaseDate)
 						{
 							// If currentShares is bigger than 0, and we're buying with a newer date
 							// we are buying additional shares, which are under wash sale rule 2.b)
 							washedShares += sharesInt;
 						}

						// Increase counter of purchases within 30 days before
						purchasesWithin30DaysBefore++;
					}

					// Increase current shares
					currentShares += sharesInt;

					// If this date is more than 30 days after last purchase date, reset alreadyWashed value
					if (lastPurchaseDate.size() != 0 &&
						daysBetweenDates(it->second[Doh_KDVP_line_values_date],lastPurchaseDate) > 30)
						alreadyWashed = 0;

					// Store lastpurchase  date
					lastPurchaseDate = it->second[Doh_KDVP_line_values_date];
				}
				else
				{
					lastTransactionSale = true;

					// Sale, update current shares counts
					ss.str("");
					ss.clear();
					ss << it->second[Doh_KDVP_line_values_shares];
					unsigned sharesInt;
					ss >> sharesInt;

					// Same day sale?+
					daysDiff = daysBetweenDates(currLineValues[Doh_KDVP_line_values_date], it->second[Doh_KDVP_line_values_date]);
					if (daysDiff == 0)
						sameDaySaleCounter++;

					currentShares -= sharesInt;
					if (currentShares < 0)
					{
						cout << __FUNCTION__ << ":" << __LINE__ << ": "
							<< "Error. Negative current shares count: " << currentShares
							<< "Ticker: " << it->second[Doh_KDVP_line_values_ticker]
							<< ", transaction id: " << it->second[Doh_KDVP_line_values_trans_id]
							<< ", date: " << it->second[Doh_KDVP_line_values_date] << endl;

						return 1;
					}

					if (washedShares > 0)
					{
						if (sharesInt > washedShares)
						{
							cout << __FUNCTION__ << ":" << __LINE__ << ": "
								<< "Error. Negative washed shares count: " << washedShares
								<< "Ticker: " << it->second[Doh_KDVP_line_values_ticker]
								<< ", transaction id: " << it->second[Doh_KDVP_line_values_trans_id]
								<< ", date: " << it->second[Doh_KDVP_line_values_date] << endl;

							return 1;
						}

						washedShares -= sharesInt;
					}

					// Store last sale date
					lastSaleDate = it->second[Doh_KDVP_line_values_date];
				}
			}
			else
			{
				// After - interested only in purchases
				if (it->second[Doh_KDVP_line_values_trans_type] != transactionTypeNames[transactionTypeBuy])
				{
					lastTransactionSale = true;
					continue;
				}

				buysCounter++;

				lastTransactionSale = false;

				// If purchase is within 30 days after, we are under rule 1
				daysDiff = daysBetweenDates(it->second[Doh_KDVP_line_values_date], currLineValues[Doh_KDVP_line_values_date]);

				if (daysDiff > 0 && daysDiff <= 30 &&
				   (lastSaleDate != currLineValues[Doh_KDVP_line_values_date]/* && alreadyWashed*/))
				{
					ss.str("");
					ss.clear();
					ss << it->second[Doh_KDVP_line_values_shares];
					ss >> sharesInt;

					washedShares += sharesInt;
				}

				// Store lastpurchase  date
				lastPurchaseDate = it->second[Doh_KDVP_line_values_date];
			}
		}

		// If washedShares is bigger than 0, we have a potential wash sale
		if (washedShares == 0)
			val = "true";
		else
		{
			ss.str("");
			ss.clear();
			ss << currLineValues[Doh_KDVP_line_values_shares];
			ss >> sharesInt;

			if (alreadyWashed >= sharesInt && buysCounter < 2)
			{
				// We already washed before more than we need to by this purchase
				val = "true";

				cout << "No wash sale... "
					<< "Ticker: " << currLineValues[Doh_KDVP_line_values_ticker]
					<< ", transaction id: " << currLineValues[Doh_KDVP_line_values_trans_id]
					<< ", date: " << currLineValues[Doh_KDVP_line_values_date]
					<< ", should wash shares: " << sharesInt
					<< ", but already washed: " << alreadyWashed << endl;

				// Decrease already washed shares
				alreadyWashed -= sharesInt;
			}
			else
			{
				// Special case - If we're selling all remaining shares at once,
				// and the last transaction is sale
				if ((currentShares == sharesInt) &&
					((lastTransactionSale && buysCounter < 2) ||
					 (lastTransactionSale && transThis > transThat)))
				{
					val = "true";
/*
					cout << "No wash sale... "
						<< "Ticker: " << currLineValues[Doh_KDVP_line_values_ticker]
						<< ", transaction id: " << currLineValues[Doh_KDVP_line_values_trans_id]
						<< ", date: " << currLineValues[Doh_KDVP_line_values_date]
						<< ", selling all shares in one sale: " << sharesInt << endl;
*/
				}
				else
				{
					// We must wash the remainder to already washed shares
					unsigned sharesToWash = washedShares - alreadyWashed;

					if (sharesToWash > 0)
					{
						val = "false";

						// We can't wash more than we're selling
						if (sharesInt < sharesToWash)
							sharesToWash = sharesInt;

						// Make a note if wash sale has been partial or complete
						cout << "Wash sale... "
							 << "Ticker: " << currLineValues[Doh_KDVP_line_values_ticker]
							 << ", transaction id: " << currLineValues[Doh_KDVP_line_values_trans_id]
							 << ", date: " << currLineValues[Doh_KDVP_line_values_date]
							 << ", washed shares: " << sharesToWash
							 << ", remaining unwashed shares: " << (sharesInt - sharesToWash) << endl;

						alreadyWashed += sharesToWash;
					}
					else
						val = "true";
				}
			}
		}
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
		// If lineElementId is not -1, this parameter is mapped from currLineValues vector
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
			val = currLineValues.at(element.lineElementId);	// No conversion
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
		    (currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuy] ||
		    currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSell]))
		{
			ret = false;
			depth = 1;
		}
	}
	else if (element.elementId == Doh_KDVP_element_SecuritiesShort)
	{
		if (ifExistingItem(element) &&
			(currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeBuyCover] ||
			currLineValues[Doh_KDVP_line_values_trans_type] == transactionTypeNames[transactionTypeSellShort]))
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
static int processElement(const elementInfo_t& element, ofstream& outputFile)
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

	// If new item is being processed, reset alreadyWashed value
	if (!ifExistingItem(element))
		alreadyWashed = 0;

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
			if (processElement(elementInfoMap.at(*it), outputFile) != 0)
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
		else if (lastElement)	// Last line, close final groups
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

// Usage: makexml.exe [help]¸[input=input_file_name] [output=output_file_name] [convert=conversion_file_name] [rate=currency]
// Examples:
// 		makexml.exe input=infile.csv output=outfile.xml convert=currency.xml rate=JPY
// 	 Using default file names and default currency conversion (USD):
//		makexml.exe convert
int main(int argc, char* argv[])
{
	static ifstream baseFile;
	static ofstream outputFile;

	// Strings from base file that should be inserted before and after our content
	string beforeBaseStr;
	string afterBaseStr;

	if (argc > 1)
	{
		// Parse arguments
		string arg;

		for (unsigned i = 1; i < argc; i++)
		{
			arg = argv[i];
			if (!arg.compare(0, 4, "help"))
			{
				cout << endl;
				cout << "  Usage: makexml.exe [help] [input=input_file_name] [output=output_file_name] [convert=conversion_file_name] [rate=currency]" << endl << endl;
				cout << "  Example: " << endl;
				cout << "      makexml.exe input=infile.csv output=outfile.xml convert=currency.xml rate=JPY" << endl;
				cout << "  Example: Using default file names and default currency conversion (USD): " << endl;
				cout << "      makexml.exe convert" << endl << endl;

				return 0;
			}
			else if (!arg.compare(0, 7, "convert"))
			{
				useCurrConversion = true;

				if (arg.length() == 7)
					continue;			// Default conversion
				else
				{
					// Conversion file
					currConvFileName = arg.substr(arg.find_first_of("=")+1);
				}
			}
			else if (!arg.compare(0, 6, "input="))
			{
				// Input file
				inputFileName = arg.substr(arg.find_first_of("=")+1);
			}
			else if (!arg.compare(0, 7, "output="))
			{
				// Output file
				outputFileName = arg.substr(arg.find_first_of("=")+1);
			}
			else if (!arg.compare(0, 5, "rate="))
			{
				// Rate
				tagTecaj = arg.substr(arg.find_first_of("=")+1);
			}
		}
	}

	cout << "  inputFileName: " << inputFileName << endl;
	cout << "  outputFileName: " << outputFileName << endl;
	cout << "  currConvFileName: " << currConvFileName << endl;

	if (useCurrConversion)
	{
		// Transform currency conversion file to local storage
		if (!conversionFileToLocalStorage(currConvFileName))
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Error converting currency file to local storage!" << endl;

			return 1;
		}
		cout << "  Using currency conversion to: " << tagTecaj << endl;
	}
	else
		cout << "  No currency conversion" << endl;

	cout << endl;

	// Convert input file to local storage
	if (!inputFileToLocalStorage(inputFileName))
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
			<< "Error converting input file to local storage!" << endl;

		return 1;
	}

	// Open base and output files
	baseFile.open(baseFileName.c_str(), ifstream::in);
	outputFile.open(outputFileName.c_str(), ofstream::out|ofstream::binary);

	if (!baseFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open base file!" << endl;
		return 1;
	}
	else if (!outputFile.is_open())
	{
		cout << __FUNCTION__ << ":" << __LINE__ << ": "
				<< "Cannot open output file!" << endl;
		return 1;
	}

	// Set before and after strings
	setBaseStr(beforeBaseStr, afterBaseStr, baseFile);

	// Copy "before" string into file
	outputFile << beforeBaseStr;

	// The main loop - go through all transactions and insert them into XML
	for (transactionMap_t::iterator it1 = transactionMap.begin(); it1 != transactionMap.end(); it1++)
	{
		// Mind that this is a map-in-map-in-map first sorted by ticker,
		// then by transaction type (long, short) and then by transaction id
		// Error check - the inside map should not be empty
		if (it1->second.size() == 0)
		{
			cout << __FUNCTION__ << ":" << __LINE__ << ": "
					<< "Transaction map empty for ticker: " << it1->first << endl;
			return 1;
		}

		for (tradeDirectionMap_t::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
		{
			// Go through both long and short trades
			for (transactionIdMap_t::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++)
			{
				// Set current line
				currLineValues = it3->second;

				// Make a note if the very last element is being processed (to close all the groups)
				transactionMap_t::iterator it11 = it1;
				tradeDirectionMap_t::iterator it21 = it2;
				transactionIdMap_t::iterator it31 = it3;
				if (++it31 == it2->second.end() && ++it21 == it1->second.end() && ++it11 == transactionMap.end())
					lastElement = true;

				// Start processing from rootElement
				if (elementInfoMap.find(rootElement) == elementInfoMap.end())
				{
					cout << __FUNCTION__ << ":" << __LINE__ << ": "
							<< "Cannot find root element " << rootElement << endl;
					return 1;
				}

				if (processElement(elementInfoMap.at(rootElement), outputFile) != 0)
				{
					cout << __FUNCTION__ << ":" << __LINE__ << ": "
							<< "Error processing root element " << rootElement << endl;
					return 1;
				}
			}
		}
	}

	// Copy "after" string into file
	outputFile << afterBaseStr;

	// Close all files
	baseFile.close();
	outputFile.close();

	return 0;
}

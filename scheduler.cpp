#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
using namespace std;
enum Type {Concert, Games, Convention, Conference}; //enum declared so that types only takes 4 values, in rest it throws error

//structure for reservations(attribute for venues)
struct Reservation{
    string cname;
    string startdate;
    string enddate;
};

//function to extract quoted part to iss in istringstream
bool readQuotedString(istringstream &iss, string &result){
    result.clear();
    char ch;

    while (iss >> ws && (iss.peek() == ' ' || iss.peek() == '\t')){
        iss.get(ch);
    }

    if (iss.peek() == '"'){
        iss.get(ch);
        getline(iss, result, '"');
        if (iss.peek() == ' ' || iss.peek() == '\t')
        {
            iss.get(ch);
        }
        return true;
    }

    if (iss >> result){
        return true;
    }

    return false;
}

//function that adds specified amount of minutes and adjusts the date+time string using ctime
string addMinutes(const string& datetime, int minutesToAdd) {
    //parsing the string into components
    int year, month, day, hour, minute;
    sscanf(datetime.c_str(), "%d-%d-%d-%d:%d", &year, &month, &day, &hour, &minute);

    //create a struct tm with the given values
    tm timeStruct = {};
    timeStruct.tm_year = year - 1900; //years since 1900
    timeStruct.tm_mon = month - 1;    //months since jan
    timeStruct.tm_mday = day;
    timeStruct.tm_hour = hour;
    timeStruct.tm_min = minute;
    timeStruct.tm_sec = 0;

    //convert to time_t (seconds since epoch)
    time_t timeEpoch = mktime(&timeStruct);

    //add the minutes
    timeEpoch += minutesToAdd * 60; // convert minutes to seconds

    //convert back to struct tm
    tm* newTimeStruct = localtime(&timeEpoch);

    //convert the updated struct tm back into a string manually
    ostringstream oss;
    oss << (newTimeStruct->tm_year + 1900) << "-"
        << (newTimeStruct->tm_mon + 1 < 10 ? "0" : "") << (newTimeStruct->tm_mon + 1) << "-"
        << (newTimeStruct->tm_mday < 10 ? "0" : "") << newTimeStruct->tm_mday << "-"
        << (newTimeStruct->tm_hour < 10 ? "0" : "") << newTimeStruct->tm_hour << ":"
        << (newTimeStruct->tm_min < 10 ? "0" : "") << newTimeStruct->tm_min;

    return oss.str();
}

//function to check if the year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//function to check if a given date is valid
bool isValidDate(int year, int month, int day) {
    if (month < 1 || month > 12 || day < 1) {
        return false;
    }

    const int daysInMonth[] = { 31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return day <= daysInMonth[month - 1];
}

//function to compare two dates
bool isGreaterDate(int year, int month, int day) {
    //get current date
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    // Compare dates
    if (year > currentYear) return true;
    if (year == currentYear && month > currentMonth) return true;
    if (year == currentYear && month == currentMonth && day > currentDay) return true;

    return false;
}

//function to validate and compare the input date with today's date
bool isValidAndFutureDate(const string& dateStr) {
    int year, month, day;
    char dash1, dash2;

    istringstream iss(dateStr);
    if (!(iss >> year >> dash1 >> month >> dash2 >> day) || dash1 != '-' || dash2 != '-' || !isValidDate(year, month, day)) {
        return false;
    }

    return isGreaterDate(year, month, day);
}

//fuctions to split strings using delimiter
vector<string> splitstring(const string& str, char delimiter) {
    if (str.empty()){
        return vector<string>();
    }
    vector<string> tokens;
    string token;
    for (char ch : str) {
        if (ch == delimiter) {
            tokens.push_back((token));
            token.clear();
        } 
        else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back((token));
    }
    return tokens;
}

bool isValidTime(const string& time) {
    int hour, minute;
    char colon;
    if (time.length() != 5 || time[2] != ':'){ //format checker
        return false;
    }
    istringstream iss(time);
    //parse the time string in the format HH:MM
    if (!(iss >> hour >> colon >> minute) || colon != ':' || iss.fail()) {
        return false;
    }

    //check if hour is between 0 and 23
    if (hour < 0 || hour > 23) {
        return false;
    }

    //check if minutes are a multiple of 15
    if (minute % 15 != 0 || minute < 0 || minute >= 60) {
        return false;
    }

    return true;
}

//convert the declared enum to string for printing
string typeToString(Type type) {
    switch(type) {
        case Concert: return "Concert";
        case Games: return "Games";
        case Convention: return "Convention";
        case Conference: return "Conference";
        default: return "Unknown";
    }
}

class Congregation{
    private:
        string cname;
        Type ctype;
        string cstartdate;
        string cenddate;

    public:
        Congregation(string inputname = "", string inputtype = "", string inputstartdate = "2024-12-31", string inputenddate = "2024-01-01"){
            if (inputtype == "Concert") ctype = Concert;
            else if (inputtype == "Games") ctype = Games;
            else if (inputtype == "Convention") ctype = Convention;
            else if (inputtype == "Conference") ctype = Conference;
            else{
                cout << "-1" << endl << "Error" << endl;
                return;
            }
            cname = inputname;
            cstartdate = inputstartdate;
            cenddate = inputenddate;
        }

    friend class CongregationList;
    friend class VenueList;
    friend class EventList;
};

class CongregationList{
    private:
        vector<Congregation> congregationlist;
    
    public:
    
        CongregationList(){
        }

        //fuction to add congregation
        void addCongregation(string inputname, string inputtype, string inputstartdate, string inputenddate){
            //problem assumed that all congregations names are unique
            for (auto element : congregationlist){
                if(element.cname == inputname){
                    cout << "-1" << endl << "Error" << endl;
                    return;
                }
            }
            Congregation newCongregation = Congregation(inputname, inputtype, inputstartdate, inputenddate);
            congregationlist.push_back(newCongregation);
            cout << 0 << endl;
            return;
        }

        //function to delete congregation
        void deleteCongregation(string inputname){
            //check if the congregation to be deleted exists 
            for(size_t i=0;i<congregationlist.size();i++){
                if(congregationlist[i].cname == inputname){
                    congregationlist.erase(congregationlist.begin() + i);
                    cout << 0 << endl;
                    return;
                }
            }
            cout << -1 << endl << "Error" << endl;
            return;
        }

        //function to display all the congregations
        void showCongregations(){
            if (congregationlist.empty()){
                cout << 0 << endl;
                return;
            }
            //return if no congregations otherwise print details of each 
            cout << congregationlist.size() << endl;
            for(auto element : congregationlist){
                cout << element.cname << " " << typeToString(element.ctype) << " " << element.cstartdate << " " << element.cenddate << endl;
            }
            return;
        }
    
    friend class VenueList;
    friend class Eventlist;
}; 

class Venue{
    private:
        string name, address, city, state, postal, country;
        int capacity;
        vector <Reservation> reservations;//contains all the reservations of a particular venue
    
    public:
        Venue(string name= "", string address= "", string city = "", string state = "", string postal = "", string country = "", int capacity = 0){
            this->country = country;
            this->name = name;
            this->address = address;
            this->city = city;
            this->state = state;
            this->postal = postal;
            this->capacity = capacity;
        }

    friend class VenueList;
    friend class EventList;
};

class VenueList{
    private:
        vector<Venue> venuelist;
        CongregationList congregations;//this object is creted here for easy iteration
    
    public:
        VenueList(){
        }
        
        //fuction to add venue 
        void addVenue(string venuename, string address, string city, string state, string postal, string country, int venuecapacity){
            //check for duplicates
            for(auto element : venuelist){
                if(element.name == venuename){
                    cout << "-1" << endl << "Error" << endl;
                    return;
                }
            }
            Venue newVenue = Venue(venuename, address, city, state, postal, country, venuecapacity);
            venuelist.push_back(newVenue);
            cout << 0 << endl;
            return;
        }
        
        //function to delete venue
        void deleteVenue(string venuename, string country){
            //check if list is empty
            if(venuelist.empty()){
                cout << -1 << endl << "Error" << endl;
                return;
            }
            //check if the venue to be deleted exists 
            for(size_t i=0;i<venuelist.size();i++){
                if(venuelist[i].name == venuename && venuelist[i].country == country && venuelist[i].reservations.empty()){
                    venuelist.erase(venuelist.begin() + i);
                    cout << 0 << endl;
                    return;
                }
            }
            cout << -1 << endl << "Error" << endl;
            return;
        }

        //function to print all existing venuelist
        void showVenues(string city, string state, string postal, string country){
            //printing total number of venuelist
            if (venuelist.empty()){
                cout << 0 << endl;
                return;
            }
            int count = 0;
            string venuelisttoprint = "\n";
            for(auto element : venuelist){
                //takes care of all types of showVenue as if not empty string then it compares otherwise its true
                if((city == "" ? true:element.city == city && element.state == state) && (postal == "" ? true:element.postal == postal) && element.country == country){
                    count++;
                    venuelisttoprint += element.name + " " + element.address + ":" + element.city + ":" + element.state + ":" + element.postal + ":" + element.country + " "+ to_string(element.capacity) + "\n";
                }
            }
            cout << count << venuelisttoprint;
            return;
        }
    
        void reserveVenue(string venuename, string country, string cname){
            //search for the congregation
            for(auto celement : congregations.congregationlist){
                if (celement.cname == cname){
                    //search for the venue
                    for(auto &velement : venuelist){
                        if(velement.name == venuename && velement.country == country){
                            for(auto element : velement.reservations){
                                //check if available for reservation or not
                                if((celement.cstartdate>=element.startdate && celement.cstartdate<=element.enddate) || (celement.cenddate>=element.startdate && celement.cenddate<=element.enddate)){
                                    cout << -1 << endl << "Error" << endl;
                                    return;
                                }
                            }
                            velement.reservations.push_back({celement.cname,celement.cstartdate,celement.cenddate});
                            cout << "0" << endl;
                            return;
                        }
                    }
                }
            }
            cout << "-1" << endl << "Error" << endl;
            return;
        }

        void freeVenue(string venuename, string country, string cname){
            //search for the venue
            for(auto &velement : venuelist){
                if(velement.name == venuename && velement.country == country){
                    //check if the particular reservation is present
                    for(size_t i=0;i<velement.reservations.size();i++){
                        if(velement.reservations[i].cname == cname){
                            velement.reservations.erase(velement.reservations.begin()+i);
                            cout << 0 << endl;
                            return;
                        }
                    }
                }
            }
            cout << -1 << endl << "Error" << endl;
            return;
        }

        void showReserved(string cname){
            //finding the congregation
            bool cfound = false;
            for(auto element : congregations.congregationlist){
                if (element.cname == cname){
                    cfound = true;
                    break;
                }
            }
            if (!cfound){
                cout << -1 << endl << "Error" << endl;
                return;
            }
            int count = 0;
            string venuelisttoprint = "";
            //prepares a string that prints all the relevant information in only one iteration
            for(auto element : venuelist){
                for(auto reservation : element.reservations){
                    if(reservation.cname == cname){
                        venuelisttoprint += element.name + " " + element.address + ":" + element.city + ":" + element.state + ":" + element.postal + ":" + element.country + " " + to_string(element.capacity) + "\n";
                        count++;
                        break;
                    }
                }
            }
            cout << count << endl << venuelisttoprint;
        }
    
        void deleteCongregationFromCongregations(string cname){
            //delete all the reservations of the congregation
            for(auto &element : venuelist){
                for(size_t i=0;i<element.reservations.size();i++){
                    if(element.reservations[i].cname == cname){
                        element.reservations.erase(element.reservations.begin() + i);
                        break;
                    }
                }
            }
            congregations.deleteCongregation(cname);
            return;
        }

    friend class EventList;
};

class Event{
    private:
    string eventname;
    string venuename;
    string cname; 
    string country;
    string date;
    string fromhour, tohour;
    
    public:
        Event(string cname = "", string venuename = "", string country = "", string eventname = "", string date = "2024-12-31", string fromhour = 0, string tohour = 0){
            this->cname = cname;
            this->venuename = venuename;
            this->country = country;
            this->eventname = eventname;
            this->date = date;
            this->fromhour = fromhour;
            this->tohour = tohour;
        }

    friend class EventList; 
};

class EventList{
    private:
        vector<Event> eventlist;
        VenueList venues;//this object is creted here for easy iteration
    
    public:
        EventList(){
            eventlist = {};
        }

        //function to add event
        void addEvent(string cname, string inputvenuename, string country, string inputdate, string inputfromhour, string inputtohour, string inputeventname){
            //check if venue and reservation of the event exists or not
            bool reserved = false;
            for(auto &velement : venues.venuelist){
                if(velement.name == inputvenuename && velement.country == country){
                    for(auto relement : velement.reservations){
                        if(relement.cname == cname && relement.startdate<=inputdate && relement.enddate>=inputdate){
                            reserved = true;
                            break;
                        }
                    }
                }
                if(reserved){
                    break;
                }
            }
            if(!reserved){
                cout << -1 << endl << "Error" << endl;
                return;
            }
            if(eventlist.empty()){
                eventlist.push_back(Event(cname, inputvenuename, country, inputeventname, inputdate, inputfromhour, inputtohour));
                cout << 0 << endl;
                return;
            }
            //iterate till the date just before the event date
            size_t pos = 0;
            for(size_t i=0;i<eventlist.size();i++){
                if((eventlist[i].date+'-'+eventlist[i].tohour)  <= (inputdate+'-'+inputfromhour)){
                    pos++;
                }
                if(eventlist[i].country == country && eventlist[i].venuename == inputvenuename){
                    string toaddstart = inputdate+string("-")+inputfromhour;
                    string toaddend = inputdate+string("-")+inputtohour;
                    string eventstart = eventlist[i].date+string("-")+eventlist[i].fromhour;
                    string eventend = eventlist[i].date+string("-")+eventlist[i].tohour;
                    //check if events have enough gap
                    if (eventend.substr(11,5) == "00:00"){
                        eventend = addMinutes(eventend, 24*60);
                    }
                    if (toaddend.substr(11,5) == "00:00"){
                        toaddend = addMinutes(eventend, 24*60);
                    }
                    if((addMinutes(toaddend,30)<=eventstart) || (toaddstart>=addMinutes(eventend,30))){
                        continue;
                    }
                    else{
                        cout << -1 << endl << "Error" << endl;
                        return;
                    }
                }
            }
            if(pos >= eventlist.size() - 1){
                eventlist.push_back(Event(cname, inputvenuename, country, inputeventname, inputdate, inputfromhour, inputtohour));
            }
            else{
                eventlist.insert(eventlist.begin()+pos, Event(cname, inputvenuename, country, inputdate, inputfromhour, inputtohour, inputeventname));
            }
            cout << 0 << endl;
            return;
        }

        //function to delete event
        void deleteEvent(string cname, string inputvenuename, string country, string inputdate, string inputfromhour, string inputeventname){
            //find the event to be deleted
            for(size_t i=0;i<eventlist.size();i++){
                if(eventlist[i].country == country && eventlist[i].venuename == inputvenuename && eventlist[i].cname == cname && eventlist[i].date == inputdate && eventlist[i].eventname == inputeventname && eventlist[i].fromhour == inputfromhour){
                    eventlist.erase(eventlist.begin()+i);
                    cout << 0 << endl;
                    return;
                } 
            }
            //if event not present
            cout << -1 << endl << "Error:in fuction" << endl;
            return;
        }

        //fuction to print all events of a venue at a particular date
        void showEvents(string inputvenuename, string country, string inputdate){
            //maintains the count of events for that venue on that date
            int count = 0;
            string toprint = "\n";
            int vfound = false;
            for(auto element : venues.venuelist){
                if(element.name == inputvenuename && element.country == country){
                    vfound = true;
                }
            }
            if(!vfound){
                cout << -1 << endl << "Error" << endl;
                return;
            }
            //all the events to be printed are added to a string so as to print in one iteration
            for(auto element : eventlist){
                if(element.venuename == inputvenuename && element.country == country && element.date == inputdate){
                    toprint += element.eventname + " " + element.fromhour + " " + element.tohour + "\n";
                    count++;
                }
            }
            cout << count << toprint; 
            return;
        }

        //function for printing all the events happening at the venue for a congregation
        void showCalendar(string cname, string inputvenuename, string country){
            //checks if reservation is present or not
            int reserved = false;
            string startdate = "";
            string enddate = "";
            for(auto velement: venues.venuelist){
                if(velement.name == inputvenuename && velement.country == country){
                    for(auto relement : velement.reservations){
                        if(relement.cname == cname){
                            reserved = true;
                            startdate = relement.startdate;
                            enddate = relement.enddate;
                            break;
                        }
                    }
                }
                if(reserved){
                    break;
                }
            }

            if(!reserved){
                cout << -1 << endl << "Error" << endl;
                return;
            }
            //print schedule for each day of the reservation
            int totalcount = 0;
            string wholetoprint = "\n";
            for(string i = startdate; i<=enddate ; i = addMinutes(i+"-00:00",24*60).substr(0,10)){
                int count = 0;
                string toprint = "\n";
                for(auto element : eventlist){
                    if(element.date == i && element.venuename == inputvenuename && element.country == country){
                        toprint += element.eventname + " " + element.fromhour + " " + element.tohour + "\n";
                        count++;
                        totalcount++;
                    }
                }
                wholetoprint += i + " " + to_string(count) + toprint;
            }
            cout << totalcount << wholetoprint;
            return;
        }

        //function for redirecting to addVenue
        void addVenueToVenues(string venuename, string address, string city, string state, string postal, string country, int venuecapacity){
            venues.addVenue(venuename, address, city , state, postal, country, venuecapacity);
            return;
        }

        //function for redirecting to deleteVenue
        void deleteVenueFromVenues(string venuename, string country){
            venues.deleteVenue(venuename, country);
            return;
        }

        //function for redirecting to showVenues
        void showVenuesFromVenues(string city, string state, string postal, string country){
            venues.showVenues(city, state, postal, country);
            return;
        }

        //all the following fuctions below is used for redirecting

        void reserveVenueFromVenues(string venuename, string country, string cname){
            venues.reserveVenue(venuename, country, cname);
            return;
        }

        void freeVenueFromVenues(string venuename, string country, string cname){
            //delete all the events of that congregation on the venue
            for(size_t i=0;i<eventlist.size();){
                if(eventlist[i].venuename == venuename && eventlist[i].country == country && eventlist[i].cname == cname){
                    eventlist.erase(eventlist.begin() + i);
                }
                else{
                    i++;
                }
            }
            venues.freeVenue(venuename, country, cname);
            return;
        }

        void showReservedVenueFromVenues(string cname){
            venues.showReserved(cname);
            return;
        }

        void addCongregationToCongregations(string inputname, string inputtype, string inputstartdate, string inputenddate){
            venues.congregations.addCongregation(inputname, inputtype, inputstartdate, inputenddate);
        }

        void deleteCongregationFromCongregationsByVenues(string inputname){
            for(size_t i=0;i<eventlist.size();){
                if(eventlist[i].cname == inputname){
                    eventlist.erase(eventlist.begin() + i);
                }
                else{
                    i++;
                }
            }
            venues.deleteCongregationFromCongregations(inputname);
            return;
        }

        void showCongregationFromCongregations(){
            venues.congregations.showCongregations();
            return;
        }
};

int main(){
    EventList events;
    string input;
    while (getline(cin, input)){

        istringstream iss(input);
        string command;
        iss >> command;

        if (command == "addCongregation"){
            string name, inputtype, startdate, enddate;
            // Parse each field using readQuotedString
            if (!readQuotedString(iss, name) || !readQuotedString(iss, inputtype) || !readQuotedString(iss, startdate) || !readQuotedString(iss, enddate)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if (inputtype != "Concert" && inputtype != "Games" && inputtype != "Convention" && inputtype != "Conference") {
                cout << "-1" << endl << "Error" << endl;
                continue;
            }
            if (!name.empty() && !inputtype.empty() && !startdate.empty() && !enddate.empty() && isValidAndFutureDate(startdate) && isValidAndFutureDate(enddate) && startdate <= enddate) {
                events.addCongregationToCongregations(name, inputtype, startdate, enddate);
            } else {
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "deleteCongregation"){
            string name;
            if (!readQuotedString(iss, name)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if (!name.empty()) {
                events.deleteCongregationFromCongregationsByVenues(name);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "showCongregations"){
            events.showCongregationFromCongregations();
        }

        else if (command == "addVenue"){
            string venuename, venuelocation;
            int venuecapacity;

            if (!readQuotedString(iss, venuename) || !readQuotedString(iss, venuelocation) || !(iss >> venuecapacity)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }

            vector<string> loc = splitstring(venuelocation, ':');
            if (!venuename.empty() && !loc[0].empty() && !loc[1].empty() && !loc[2].empty() && !loc[3].empty() && !loc[4].empty() && venuecapacity > 0) {
                events.addVenueToVenues(venuename, loc[0], loc[1], loc[2], loc[3], loc[4], venuecapacity);
            } 
            else {
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "deleteVenue"){
            string venuename, country;
            if (!readQuotedString(iss, venuename) || !readQuotedString(iss, country)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if (!venuename.empty() && !country.empty()){
                events.deleteVenueFromVenues(venuename, country);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }
        
        else if (command == "showVenues"){
            string loc;
            if (!readQuotedString(iss, loc)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }

            vector<string> location = splitstring(loc, ':');
            if(!location[3].empty() && (location[0].empty() ? true:!location[1].empty())){
                events.showVenuesFromVenues(location[0], location[1], location[2], location[3]);
            }
            else{
                cout << -1 << endl << "Error" << endl; 
            }
        }

        else if (command == "reserveVenue"){
            string venuename, country, cname;
            if (!readQuotedString(iss, venuename) || !readQuotedString(iss, country) || !readQuotedString(iss, cname)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if(!venuename.empty() && !country.empty() && !cname.empty()){
                events.reserveVenueFromVenues(venuename, country, cname);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "freeVenue"){
            string venuename, country, cname;
            if (!readQuotedString(iss, venuename) || !readQuotedString(iss, country) || !readQuotedString(iss, cname)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if(!venuename.empty() && !country.empty() && !cname.empty()){
                events.freeVenueFromVenues(venuename, country, cname);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "showReserved"){
            string cname;
            if (!readQuotedString(iss, cname)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if(!cname.empty()){
                events.showReservedVenueFromVenues(cname);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "addEvent"){
            string cname, venuename, eventname, country;
            string date, fromhour, tohour;
            if (!readQuotedString(iss, cname) || !readQuotedString(iss, venuename) || !readQuotedString(iss, country) || !readQuotedString(iss, date) || !readQuotedString(iss, fromhour) || !readQuotedString(iss, tohour) || !readQuotedString(iss, eventname)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }

            if (!venuename.empty() && !eventname.empty() && isValidAndFutureDate(date) && isValidTime(fromhour) && isValidTime(tohour) &&  (addMinutes("2024-01-01-"+fromhour, 30).substr(11,5)<=tohour || (tohour == "00:00" && fromhour<="23:30"))) {
                events.addEvent(cname, venuename, country, date, fromhour, tohour, eventname);
            }
            else {
                cout << -1 << endl << "Error" << endl;
            }
        }
        
        else if (command == "deleteEvent"){
            string cname, venuename, eventname, country;
            string date, fromhour;
            if (!readQuotedString(iss, cname) || !readQuotedString(iss, venuename) || !readQuotedString(iss, country) || !readQuotedString(iss, date) || !readQuotedString(iss, fromhour) || !readQuotedString(iss, eventname)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if (!venuename.empty() && !eventname.empty() && isValidAndFutureDate(date) && isValidTime(fromhour)){
                events.deleteEvent(cname, venuename, country, date, fromhour, eventname);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }
        
        else if (command == "showEvents"){
            string venuename, country, date;
            if (!readQuotedString(iss, venuename) || !readQuotedString(iss, country) || !readQuotedString(iss, date)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if (!venuename.empty() && !country.empty() && isValidAndFutureDate(date)){
                events.showEvents(venuename, country, date);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "showCalendar"){
            string cname, venuename, country;
            if (!readQuotedString(iss, cname) || !readQuotedString(iss, venuename) || !readQuotedString(iss, country)) {
                cout << -1 << endl << "Error" << endl;
                continue;
            }
            if (!cname.empty() && !venuename.empty() && !country.empty()){
                events.showCalendar(cname, venuename, country);
            }
            else{
                cout << -1 << endl << "Error" << endl;
            }
        }

        else if (command == "End"){
            break;
        }
    }
    return 0;
}
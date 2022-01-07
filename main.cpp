#include <bits/stdc++.h>
using namespace std;
fstream schemafile;

//Check schema of database
void checkSchema(string table, vector<string>& schema){
    schemafile.open("Schema.txt", ios::in);
    string line;
    while(getline(schemafile, line)){
        string l1 = line.substr(0, line.find('#'));
        if(table == l1){
            string tab = line;
            int i=0;
            string curr = "";
            while(i < tab.length()){
                if(tab[i] == '#'){
                    schema.push_back(curr);
                    curr = "";
                }else
                    curr += tab[i];
                i++;
            }
            schema.push_back(curr);
            break;
        }
    }
    schemafile.close();
}
//Create table
void createTable(vector<string> cmd) {
    if(cmd.size() == 3){
        cout<<"Missing left parenthesis"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }
    if(cmd.size() < 7){
        cout<<"Invalid indentifier"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }
    string table_name = cmd[2];
    vector<string> schema;
    checkSchema(table_name, schema);

    if(!schema.empty()){
        cout<<"Table Already Exists"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    schemafile.open("Schema.txt", ios::app);
    schemafile << table_name;

    int start = -1, end = -1;
    for (int i = 3; i < cmd.size(); i++) {
        if (cmd[i] == "(") {
            start = i;
        }
        if (cmd[i] == ")") {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        cout << "Error" << endl;
        return;
    }

    while (start < end - 1) {
        start++;
        if (cmd[start] != ",") {
            schemafile << "#" << cmd[start];
        }
    }
    schemafile << endl;
    cout << "Table created successfully" << endl;
    fstream table;
    table_name += ".txt";
    const char *c = table_name.c_str();
    table.open(c, ios::app);
    schemafile.close();
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Drop table
void dropTable(vector<string> cmd){
    if(cmd.size() == 2) {
        cout<<"Table name is not mentioned"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    string table_name = cmd[2];
    vector<string> schema;
    checkSchema(table_name, schema);
    if(schema.empty()){
        cout << "Table is not already exist" << endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    schemafile.open("Schema.txt", ios::in);
    fstream temp;
    temp.open("temp.txt", ios::out);
    string line;

    while(getline(schemafile, line)) {
        string l1 = line.substr(0, line.find('#'));
        if (table_name != l1) {
            temp << line << endl;
        }
    }

    schemafile.close();
    temp.close();

    remove("Schema.txt");
    rename("temp.txt", "Schema.txt");
    fstream table;
    table_name += ".txt";
    const char *c = table_name.c_str();
    remove(c);
    cout << "Table dropped successfully" << endl;
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Truncate data
void truncate_table(vector<string> cmd) {
    if(cmd.size() == 2) {
        cout<<"Table name is not mentioned"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    string table_name = cmd[2];
    vector<string> schema;
    checkSchema(table_name, schema);
    if(schema.empty()){
        cout << "Table is not already exist" << endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    fstream table;
    fstream temp;
    temp.open("temp.txt", ios::out);
    table_name += ".txt";
    const char *c = table_name.c_str();
    table.open(c, ios::out);
    temp.close();
    table.close();
    remove(c);
    rename("temp.txt", c);
    cout << "Data deleted successfully" << endl;
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Describe table
void describe(vector<string> cmd){
    schemafile.open("Schema.txt", ios::in);

    if (cmd.size() > 1) {
        string table_name = cmd[1];
        string line;

        while (getline(schemafile, line)) {
            string l1 = line.substr(0, line.find('#'));

            if (table_name == l1) {
                cout<<"Table Name\t ---> \t"<<table_name<<endl;
                cout<<"Column\t\t"<<"Data type"<<endl; 
                string l2 = line.substr(line.find('#') + 1, line.size());
                int cnt = 1;

                for (int i=0; i<l2.length(); i++) {
                    if (l2[i] == '#') {
                        if (cnt % 2 != 0) {
                            cout << "\t-->\t";
                        } else {
                            cout << endl;
                        }
                        cnt++;
                    } else {
                        cout << l2[i];
                    }
                }
            }
        }
    } else{
        cout<<"Object to be described could not be found."<<endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Checking the given string is number or not
bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) 
        it++;
    return !s.empty() && it == s.end();
}

//Finding datatype 
void getDatatype(string tableName,vector<string>& data, vector<string> schema){
    for(int i=2;i<schema.size();i+=2){
        data.push_back(schema[i]);
    }
}

//Insert function
void insert_Into(vector<string> cmd){
    string table_name = cmd[2];
    vector<string> schema;
    checkSchema(table_name, schema);
    if(schema.empty()){
        cout << "Table is not already exist" << endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    fstream table;
    table_name += ".txt";
    const char *c = table_name.c_str();
    table.open(c, ios::app);

    int start = -1, end = -1;
    for (int i=4; i<cmd.size(); i++) {
        if (cmd[i] == "(") {
            start = i;
        }
        if (cmd[i] == ")") {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        cout << "Error" << endl;
        return;
    }

    int k=0; 
    bool invalidType=false;
    int st=start;
    vector<string> data;
    getDatatype(table_name, data, schema);

    while (st < end - 1) {
        st++;
        if (cmd[st] != ",") {
            bool flag_int = is_number(cmd[st]);
            if( ( data[k]!="int" && flag_int ) || 
                ( data[k]=="int" && !flag_int )){
                cout<<"Invalid Data Type "<<cmd[st]<<endl;
                cout << "\n-----------------------------------------------------------------" << endl;
                return;
            }
            k++;
        }
    }

    while (start < end - 1) {
        start++;
        if (cmd[start] != ",") {
            table << cmd[start];
            if (start != end - 1) {
                table << "#";
            } else {
                table << "\n";
            }
        }
    }
    schemafile << endl;
    cout << "Tuple inserted successfully" << endl;
    schemafile.close();
    cout << "\n-----------------------------------------------------------------" << endl;
}
//check cloumn number in table
void table_number(map<string, int>& table, vector<string> schema){
    int cnt = 0;
    for(int j=1; j<schema.size(); j+=2){
        table[schema[j]] = cnt;
        cnt++;
    }
}

//Check column of table
void checkCommand(map<int, string>& column_change, vector<string> cmd, vector<string> schema){
    int i;
    map<string, int> table;
    table_number(table, schema);
    for(i=3; i<cmd.size() && cmd[i]!="where"; i+=3){
        int col = table[cmd[i]];
        column_change[col] = cmd[i+2];
    }
}
//Select function
void select(vector<string> cmd) {
    vector<string>::iterator it;
    it = find(cmd.begin(), cmd.end(), "from");

    if(it != cmd.end()){
        int idx_table = 0;
        idx_table = it - cmd.begin();
        string table_name = cmd[idx_table+1];
        vector<string> schema;
        checkSchema(table_name, schema);

        if(!schema.empty()){
            table_name += ".txt";
            fstream table;
            const char *c = table_name.c_str();
            table.open(c, ios::in);
            vector<string>::iterator it_where;
            it_where = find(cmd.begin(), cmd.end(), "where");
            vector<int> attribute;

            string line;
            int i, j=0;
            int flag = 0;
            int count = 0;
            vector<string> lineVec;
            map<string, int> table_num;
            table_number(table_num, schema);

            //Select all attributes
            if(cmd[1] == "*"){
                for(i=1; i<schema.size(); i+=2)
                    attribute.push_back(table_num[schema[i]]);
            }
            //Select attributes which mentioned in the query
            else{
                for(i=1; i<idx_table; i+=2){
                    attribute.push_back(table_num[cmd[i]]);
                }
            }
            
            //where condition is not in command
            if(it_where == cmd.end()){
                while(getline(table, line)){
                    string tab = line;
                    i=0;
                    string curr = "";
                    while(i<tab.length()){
                        if(tab[i] == '#'){
                            lineVec.push_back(curr);
                            curr = "";
                        }else{
                            curr += tab[i];
                        }
                        i++;
                    }
                    lineVec.push_back(curr);

                    string new_line = "";
                    for(i=0; i<attribute.size(); i++){
                        new_line += lineVec[attribute[i]];
                        new_line += "  ---  ";
                    }
                    cout<<new_line<<endl;
                    lineVec.clear();
                }
            }
            //where condition is in command
            else{
                while (getline(table, line)) {
                    string tab = line;
                    i=0;
                    string curr = "";
                    while(i<tab.length()){
                        if(tab[i] == '#'){
                            lineVec.push_back(curr);
                            curr = "";
                        }else{
                            curr += tab[i];
                        }
                        i++;
                    }
                    lineVec.push_back(curr);

                    int idx = 0;
                    idx = it_where - cmd.begin();
                    j=0;
                    for(i=1; i<schema.size(); i+=2){
                        if(cmd[idx+1] == schema[i]){
                            //Equality condition
                            if(cmd[idx+2] == "="){
                                if(lineVec[j] == cmd[idx+3]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            //Greater than condition
                            if(cmd[idx+2] == ">"){
                                if(lineVec[j] > cmd[idx+3]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            //Greater than or equal to
                            if(cmd[idx+2] == ">="){
                                if(lineVec[j] >= cmd[idx+3]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            //smaller than condition
                            if(cmd[idx+2] == "<"){
                                if(lineVec[j] < cmd[idx+3]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            //smaller than and equal to
                            if(cmd[idx+2] == "<="){
                                if(lineVec[j] <= cmd[idx+3]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            //Not equal to condition
                            if(cmd[idx+2] == "!="){
                                if(lineVec[j] != cmd[idx+3]){
                                    flag = 1;
                                    count++;
                                }
                            }
                        }
                        j++;
                    }
                    // select proper rows from table
                    if(flag == 1){
                        string new_line = "";
                        for(i=0; i<attribute.size(); i++){
                            new_line += lineVec[attribute[i]];
                            new_line += "  ---  ";
                        }
                        cout<<new_line<<endl;
                    }
                    flag = 0; 
                    lineVec.clear();
                }
            }
            table.close();
        } else {
            cout << "Table is not already exist" << endl;
        }
    }else {
        cout<<"Syntax error [Table is not mentioned]"<<endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}
//Update function
void update_set(vector<string> cmd) {
    vector<string> schema;
    checkSchema(cmd[1], schema);

    if(!schema.empty()){
        string table_name = cmd[1] + ".txt";
        fstream table;
        const char *c = table_name.c_str();
        table.open(c, ios::in);
        vector<string>::iterator it;
        it = find(cmd.begin(), cmd.end(), "where");
        map<int, string> column_change;
        checkCommand(column_change, cmd, schema);

        string line;
        vector<string> lineVec;
        int i, j=0;
        int flag = 0;
        int count = 0;
        fstream temp;
        temp.open("temp.txt", ios::out);
        //where condition is not in command
        if(it == cmd.end()){
            while(getline(table, line)){
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                string new_line = "";
                map<int, string>::iterator itr;
                for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                    lineVec[itr->first] = itr->second;
                }

                for(i=0; i<lineVec.size()-1; i++){
                    new_line += lineVec[i]; 
                    new_line += "#";
                }
                new_line += lineVec[i];
                temp << new_line << endl;
                lineVec.clear();
            }
        }
        //where condition is in command
        else{
            while (getline(table, line)) {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                int idx = 0;
                idx = it - cmd.begin();
                j=0;
                for(i=1; i<schema.size(); i+=2){
                    if(cmd[idx+1] == schema[i]){
                        //Equality condition
                        if(cmd[idx+2] == "="){
                            if(lineVec[j] == cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than condition
                        if(cmd[idx+2] == ">"){
                            if(lineVec[j] > cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than or equal to
                        if(cmd[idx+2] == ">="){
                            if(lineVec[j] >= cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than condition
                        if(cmd[idx+2] == "<"){
                            if(lineVec[j] < cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than and equal to
                        if(cmd[idx+2] == "<="){
                            if(lineVec[j] <= cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Not equal to condition
                        if(cmd[idx+2] == "!="){
                            if(lineVec[j] != cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                } 
                //Not making any changes in the row
                if(flag != 1){
                    temp << line << endl;
                    lineVec.clear();
                }
                //make changes in a row 
                else{
                    string new_line = "";
                    map<int, string>::iterator itr;
                    for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                        lineVec[itr->first] = itr->second;
                    }

                    for(i=0; i<lineVec.size()-1; i++){
                        new_line += lineVec[i]; 
                        new_line += "#";
                    }
                    new_line += lineVec[i];
                    temp << new_line << endl;
                    lineVec.clear();
                }
                flag = 0;
            }  
        }
        table.close();
        temp.close();
        remove(c);
        rename("temp.txt", c);
        cout<<"Rows updated successfully"<<endl;
    } else {
            cout << "Table is not already exist" << endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Delete function
void delete_data(vector<string> cmd) {
    vector<string> schema;
    checkSchema(cmd[2], schema);

    if(!schema.empty()){
        if(cmd.size() == 3){
            string table_name = cmd[2] + ".txt";
            fstream table;
            const char *c = table_name.c_str();
            remove(c);
            table.open(c, ios::app);
            cout<<"Rows deleted successfully"<<endl;
        }else if(cmd[3] == "where"){
            string table_name = cmd[2] + ".txt";
            fstream table;
            const char *c = table_name.c_str();
            table.open(c, ios::in);

            string line;
            int i,j=0;
            int flag = 0;
            int count = 0;
            vector<string> lineVec;
            fstream temp;
            temp.open("temp.txt", ios::out);
            
            while(getline(table, line)){
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);
                
                for(i=1; i<schema.size(); i+=2){
                    if(cmd[4] == schema[i]){
                        //Equality condition
                        if(cmd[5] == "="){
                            if(lineVec[j] == cmd[6]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than condition
                        if(cmd[5] == ">"){
                            if(lineVec[j] > cmd[6]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than or equal to
                        if(cmd[5] == ">="){
                            if(lineVec[j] >= cmd[6]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than condition
                        if(cmd[5] == "<"){
                            if(lineVec[j] < cmd[6]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than and equal to
                        if(cmd[5] == "<="){
                            if(lineVec[j] <= cmd[6]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Not equal to condition
                        if(cmd[5] == "!="){
                            if(lineVec[j] != cmd[6]){
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }
                if(flag != 1){
                    temp << line << endl;
                }
                flag = 0;
            }
            table.close();
            temp.close();
            remove(c);
            rename("temp.txt", c);
            cout<<"Rows deleted successfully"<<endl;
        }
    } else {
            cout << "Table is not already exist" << endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Help command
void helpCmd(vector<string> cmd) {
    cout << "\n------------------------ HELP ----------------------------" << endl;
    map<string, int> help;
    help["createtable"] = 1;
    help["droptable"] = 2;
    help["select"] = 3;
    help["insert"] = 4;
    help["delete"] = 5;
    help["update"] = 6;
    map<string, int>::iterator it1;
    string command = cmd[1] + (cmd.size() > 2 ? cmd[2] : "");
    it1 = help.find(command);

    switch (it1->second) {
    case 1:
        cout << "\nCommand : CREATE TABLE" << endl;
        cout << "Info: The CREATE TABLE statement is used to create a new table in a database." << endl;
        cout << "\nSyntax: \nCREATE TABLE table_name ( attribute_1 attribute1_type CHECK (constraint1), \nattribute_2 attribute2_type, PRIMARY KEY ( attribute_1, attribute_2 ), \nFOREIGN KEY ( attribute_y ) REFERENCES table_x ( attribute_t ));" << endl;
        break;

    case 2:
        cout << "\nCommand : DROP TABLE" << endl;
        cout << "Info: The DROP TABLE statement is used to drop an existing table in a database." << endl;
        cout << "\nSyntax: \nDROP TABLE table_name;" << endl;
        break;

    case 3:
        cout << "\nCommand : SELECT" << endl;
        cout << "Info: The SELECT statement is used to select data from a database." << endl;
        cout << "\nSyntax: \nSELECT column_names \nFROM table_list \nWHERE condition_list;" << endl;
        break;

    case 4:
        cout << "\nCommand : INSERT" << endl;
        cout << "Info: The INSERT statement is used to insert new records in a table." << endl;
        cout << "\nSyntax: \nINSERT INTO table_name \nVALUES ( val1, val2, ... );" << endl;
        break;

    case 5:
        cout << "\nCommand : DELETE" << endl;
        cout << "Info: The DELETE statement is used to delete existing records in a table." << endl;
        cout << "\nSyntax: \nDELETE \nFROM table_name \nWHERE condition_list;" << endl;
        break;

    case 6:
        cout << "\nCommand : UPDATE" << endl;
        cout << "Info: The UPDATE statement is used to modify the existing records in a table." << endl;
        cout << "\nSyntax: \nUPDATE table_name \nSET attr1 = val1, attr2 = val2… \nWHERE condition_list;" << endl;
        break;

    default:
        cout << "This command is not available in SQL"<<endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}

//Help tables
void helpTable(){
    string line;
    cout<<"Available tables: "<<endl;
    schemafile.open("Schema.txt");

    int count = 0;
    while (getline(schemafile, line)) {
        string l1 = line.substr(0, line.find('#'));
        cout << l1 << endl;
        count++;
    }
    if (!count) {
        cout << "No tables found" << endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}

void handleCmd(vector<string> cmd){
    //Create table
    if (cmd[0] == "create" && cmd[1] == "table") {
        createTable(cmd);
    }
    //Drop table
    else if (cmd[0] == "drop" && cmd[1] == "table") {
        dropTable(cmd);
    } 
    //Describe
    else if (cmd[0] == "describe") {
        describe(cmd);
    }
    //Insert
    else if (cmd[0] == "insert" && cmd[1] == "into") {
        insert_Into(cmd);
    }
    //Select
    else if (cmd[0] == "select") {
        select(cmd);
    }
    //Update
    else if (cmd[0] == "update" && cmd[2] == "set") {
        update_set(cmd);
    }
    //Delete
    else if (cmd[0] == "delete") {
        delete_data(cmd);
    }
    //Truncate
    else if (cmd[0] == "truncate" && cmd[1] == "table") {
        truncate_table(cmd);
    }
    //Help cmd
    else if (cmd[0] == "Help" && cmd[1] != "table") {
        helpCmd(cmd);
    }
    //Help table
    else if (cmd[0] == "Help" && cmd[1] == "table") {
        helpTable();
    }
    //Wrong command
    else {
        cout <<"Syntax Error (Wrong command)"<<endl;
    }
}

void convertToVector(string input, vector<string> &v){
    string word = "";
    int i=0;
    while(i<input.length()){
        if(input[i] == ' '){
            v.push_back(word);
            word = "";
        }else{
            word = word + input[i];
        }
        i++;
    }
    v.push_back(word);
}

int main() {
    cout<<endl;
    cout << "\n-----------------------------------------------------------------\n" << endl;
    cout<<"\t     Group No. :- 39\t\n\tDatabase Management System\t\n"<<endl;
    cout<<"Group members :- \n\t1. Tanaya Gogawale -- 07\n\t2. Aditya Honrao -- 17\n\t3. Sayali Kadam -- 33\n\t4. Shweta Lokhande -- 70"<<endl;

    cout << "\n-----------------------------------------------------------------\n" << endl;
    vector<string> cmd;
    string input;
    cout<<"Enter your SQL command: "<<endl;
    cout<<"\t\t1. create table\n\t\t2. Drop table\n\t\t3. Describe\n\t\t4. Insert into\n\t\t5. Select\n\t\t6. Update data\n\t\t7. Delete table\n\t\t8. Truncate table\n\t\t9. Help table\n\t\t10. Help\n\t\t11. Quit"<<endl;
    getline(cin, input);
    while (input != "Quit") {
        convertToVector(input, cmd);
        handleCmd(cmd);
        cmd.clear();
        cout<<endl;
        cout<<"Enter your SQL command: "<<endl;
        cout<<"\t\t1. create table\n\t\t2. Drop table\n\t\t3. Describe\n\t\t4. Insert into\n\t\t5. Select\n\t\t6. Update data\n\t\t7. Delete table\n\t\t8. Truncate table\n\t\t9. Help table\n\t\t10. Help\n\t\t11. Quit"<<endl;
        getline(cin, input);
    }
    cout << "\n-----------------------------------------------------------------" << endl;
    cout<<endl;
}
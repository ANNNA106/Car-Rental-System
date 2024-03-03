#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <iterator>
#include <unistd.h>

using namespace std;

void write();
void read();
void car_rental_system();

int car_num = 0, emp_i=0, cus_i=0;
set<int> emp_ids,cus_ids,car_ids;
set<int> available_emp_ids, available_cus_ids, available_car_ids;

void drawline(char x){
    cout<<endl;
    for(int i=0; i<80; i++) cout<<x;
    cout<<endl;
}

// i should be the id of the last user in the table, if no user is registered i is 0

class Cars{
    public:
        int cost;
        int id;
        int condition;
        // conditon is on a scale of 1 to 10
        bool is_rented;
        pair<int,bool> rented_to;
        // first value is the ID
        // if second value is false then its a customer, and employee for true
        string model;
        int days;
        // if car is rented, then for how many days
        
        // carID, model, condition, cost, isrented, rented_to_Id, rented_to_customer(false)/employee(true), days of rent
        Cars(int id=-1, string model="Alcazar", int condition=10, int cost=2000, bool is_rented=false, pair<int,bool> rented_to= make_pair(-1,false), int days=0){
            this->id=id;
            this->model=model;
            this->condition=condition;
            this->cost=cost;
            this->is_rented=is_rented;
            this->rented_to=rented_to;
            this->days=days;
        }
        void view_to_user(bool is_employee){
            drawline('.');
            cout<<"\n\t Car ID:"<<id;
            cout<<"\n\t Car Model:"<<model;
            cout<<"\n\t Condition (on a scale of 1 to 10):"<<condition;
            if(!is_employee) cout<<"\n\tCost:"<<cost<<"Rs";
            else cout<<"\n\tCost (15 percent off):"<< (85*cost)/100<<"Rs"<<endl;
            cout<<"To be returned in "<<days<<" days"<<endl;
            drawline('.');
            cout<<endl;
        }
        void view_to_manager(){
            drawline('.');
            cout<<"\t\tCar ID:"<<id<<endl;
            cout<<"\t\tCar Model:"<<model<<endl;
            cout<<"\t\tCondition (on a scale of 1 to 10):"<<condition<<endl;
            cout<<"\t\tCost:"<<cost<<"Rs"<<endl;
            if(is_rented) cout<<"\t\tCar has been rented for "<<days<<" days"<<endl;
            if(is_rented && rented_to.second) cout<<"\t\tRented to employee"<<rented_to.first<<endl;
            else if(is_rented && !rented_to.second) cout<<"\t\tRented to customer"<<rented_to.first<<endl;
            else cout<<"\t\tNot rented"<<endl;
            drawline('.');
            cout<<endl;
            

        }
        void update(){
            cout<<"\n\n What do you wish to update?\n";
            cout<<"\t 1.Model\n \t2.Condition\n \t3.Cost\n \t4.Back\n";
            string op;
            cin>>op;
            try{
                switch (stoi(op))
                {
                case 1 :
                    cout<<"\nEnter new model:";
                    cin>>this->model;
                    break;
                case 2 :
                    cout<<"\nEnter new condition:";
                    cin>>this->condition;
                    break;
                case 3 :
                    cout<<"\nEnter new cost:";
                    cin>>this->cost;
                    break;
                case 4 :
                    break;
                default:
                    cout<<"\n\tEnter a valid value!\n";
                    break;
                }
            }
            catch(...){
                cout<<"\nInvalid Inputs!\n";
                cout<<" Press any key to go back\n";
                string x;
                cin>>x;
                update();
            }

        }
};
map<int,Cars> car;




class User{
    public:
        int rented_num;
        vector<int> rented_cars;
        User(){};

        void view(){
            cout<<"\n\t\t Number of cars rented right now:"<< rented_num<<endl;
            for(int j=0; j<rented_num; j++){
                cout<<"Car ID."<<rented_cars[j]<<endl;
                // cout<<"Model:"<<rented_cars[j].model<<"\t Due date:"<<rented_cars[j].due_date[0]<<"/"<<rented_cars[j].due_date[1]<<"/"<<rented_cars[j].due_date[2]<<endl;
            }
        }
        void rent_car(int id){
            rented_cars.push_back(id);

        }       
};

class Employee: public User{
    string password;
    public:
    int employee_record;
    int id;
    string name;
        Employee(string name="", string password="", int employee_record=5, int rented_num=0) : User(){
            this->id=*available_emp_ids.begin();
            this->password=password;
            this->employee_record=employee_record;
            this->rented_num=rented_num;
            this->name=name;
        }

        string view_password(){
            return this->password;
        }

        void view(){
            drawline('*');
            cout<<"\n\t\t ID:"<<id;
            cout<<"\n\t\t Name:"<<this->name;
            cout<<"\n\t\t Employee record (Maximum rentable cars):"<<employee_record;
            cout<<"\n\t\t The cars rented by the Employee:\n";
            for(auto c:rented_cars){
                cout<<c<<endl;
                car[c].view_to_manager();
            }
            drawline('*');
        }

        void update(){
            cout<<"Update the employee data"<<endl;
        }

        void rent_car(int car_id, int days){
            cout<<"\tcar with id: "<<car_id<<" rented \n";
            car[car_id].is_rented = true;
            car[car_id].rented_to = make_pair(this->id, true);
            rented_cars.push_back(car_id);
            rented_num++;
            car[car_id].days=days;
        }
        

        void employee_dashboard(){
            system("clear");
            drawline('-');
            cout<<"\n\t\t WELCOME, "<<name<<endl;
            drawline('-');
            cout<<"\n\t\t 1.Rent a car\n";
            cout<<"\n\t\t 2.Return a car and Pay the due amount(if any)\n";
            cout<<"\n\t\t 3.View the rented cars\n";
            cout<<"\n\t\t 4.Return\n\t\t";
            drawline('-');
            string op, car_id;
            cin >> op;
            if (op == "1") {
                if(employee_record<=rented_num){
                    cout<<"\n\t\t Your past record of returning cars is very poor, so you can't rent more cars!\n";
                    cout<<"Press any key to go back:";
                    employee_dashboard();
                }
                cout << "\n\t\t AVAILABLE CARS\n";
                bool foundCar = false;
                for (auto i: car_ids) {
                    if (!car[i].is_rented) {
                        car[i].view_to_user(true);
                        foundCar = true;
                    }
                }
                if (!foundCar) {
                    cout << "\n\t\t (No available cars.)\n";
                    cout<<" Press any key to go back ";
                    string x;
                    cin>>x;
                    system("clear");
                    employee_dashboard();
                } 
                else {
                    string x;
                    cout<<"\n\t Enter the number of days for which you wish to rent:";
                    cin>>x;
                    try {
                        int days= stoi(x);
                        cout << "\n\t Enter ID of the Car you wish to rent:";
                        cin >> car_id;
                        if (car.find(stoi(car_id)) == car.end()) {
                            cout << "\n\t\t No Car with this ID exists!\n";
                            cout<<"\n Press any key to go back\n";
                            cin>>x;
                            system("clear");
                            employee_dashboard();
                        } 
                        else if(car[stoi(car_id)].is_rented){
                            car[stoi(car_id)].view_to_manager();
                            cout<<"\n\t\t The car is already rented!\n";
                            cout<<"\n Press any key to go back\n";
                            string x;
                            cin>>x;
                            employee_dashboard();
                        }
                        else {
                            rent_car(stoi(car_id), days);
                            string x;
                            cout<<"press any key to go back";
                            cin>>x;
                            system("clear");
                            employee_dashboard();
                        }
                    }
                    catch(...){
                        cout<<"\n invalid input!\n";
                        employee_dashboard();
                    }
                }
            } 
            else if (op == "2") {
                for(auto c: rented_cars){
                    car[c].view_to_user(true);
                }
                cout << "\n\t Enter Car ID to be returned:";
                cin >> car_id;
                bool present=false;
                for(auto it = rented_cars.begin(); it != rented_cars.end(); ++it){
                    if(*it==stoi(car_id)){
                        present=true;
                        rented_cars.erase(it);
                        break;
                    } 
                }
                if(present){
                    rented_num--;
                    car[stoi(car_id)].is_rented=false;
                    car[stoi(car_id)].rented_to.first=-1;
                    cout<<"car with id:"<<car_id<<"returned \n";
                } 
                else{
                    cout<<"\n You have not rented car with id: "<<car_id<<endl;
                    cout<<" Press any key to go back\n";
                    string x;
                    cin>>x;
                    employee_dashboard();
                }
                string x;
                cout<<"\n\t\t How many days has it been, after you rented this car?:";
                cin>>x;
                try{
                    int days_passed= stoi(x);
                    cout<<car[stoi(car_id)].days<<endl;
                    int fine= (max(0,days_passed - car[stoi(car_id)].days))*100;
                    if(fine){
                        cout<<"\n\t\t A fine of Rs "<<fine<<" has to be paid\n";
                        employee_record=max(0,(employee_record-1));
                        cout<<"\n\t\t The maximum number of cars you can rent now is:"<< employee_record<<endl;
                        // emp can now max rent these many cars
                    }
                    else{
                        cout<<"\n\t\t No fine is applied!\n";
                        employee_record=min(10,(employee_record+1));
                        // emp can now rent a max of these many cars (reward for returning on time)
                    }
                }
                catch(...){
                    cout<<"\n Invalid Input!\n";
                    employee_dashboard();
                }

                cout<<" Press any key to go back\b";
                cin>>x;   
                system("clear");   
                employee_dashboard();        
            } 
            else if (op == "3") {
                cout << "\n\t\t Number of cars that can be rented:" << (employee_record - rented_num) << endl;
            
                for(auto c: rented_cars){
                    car[c].view_to_user(true);
                }
                cout<<" Press any key to go back\n";
                string x;
                cin>>x;
                system("clear");
                employee_dashboard();
            } 
            else if (op == "4") {
                car_rental_system();
            }

        }

        void Register(){
            cout<<"\t\t Your ID is: "<<id<<endl;
            cout<<"\t\t Employee Registered"<<endl;
            cout<<"\t\t press any key to go back \n";
            string x;
            cin>>x;
        }
        
        void Login(){
            string password;
            cout<<"Enter Password:";
            cin>>password;
            if(this->password==password){
                system("clear");
                cout<<"\n Welcome! (the employee is logged in)\n";
                employee_dashboard();
            }
        }
};

class Customer: public User{
    string password;
    public:
    int customer_record;
    int id;
    string name;
        Customer(string name="", string password="", int customer_record=5, int rented_num=0) : User(){
            this->id=*available_cus_ids.begin();
            this->password=password;
            this->customer_record=customer_record;
            this->rented_num=rented_num;
            this->name=name;
        }
        string view_password(){
            return this->password;
        }

        void view(){
            cout<<"\n\t\t ID:"<<this->id;
            cout<<"\n\t\t Name:"<<this->name;
            cout<<"\n\t\t Customer record (Maximum rentable cars):"<<customer_record;
            cout<<"\n\t\t The cars rented by the Customer:\n";
            for(auto c:rented_cars){
                car[c].view_to_manager();
            }
        }

        void update(){
            cout<<"Update the customer data"<<endl;
        }

        void rent_car(int car_id, int days){
            cout<<"\tcar with id: "<<car_id<<" rented \n";
            car[car_id].is_rented = true;
            car[car_id].rented_to = make_pair(this->id, false);
            rented_cars.push_back(car_id);
            rented_num++;
            car[car_id].days=days;
        }
        

        void customer_dashboard(){
            system("clear");
            drawline('-');
            cout<<"\n\t\t WELCOME, "<<name<<endl;
            drawline('-');
            cout<<"\n\t\t 1.Rent a car\n";
            cout<<"\n\t\t 2.Return a car and Pay the due amount(if any)\n";
            cout<<"\n\t\t 3.View the rented cars\n";
            cout<<"\n\t\t 4.Return\n\t\t";
            drawline('-');
            string op, car_id;
            cin >> op;
            if (op == "1") {
                if(customer_record<=rented_num){
                    cout<<"\n\t\t Your past record of returning cars is very poor, so you can't rent more cars!\n";
                    cout<<"Press any key to go back:";
                    customer_dashboard();
                }

                cout << "\n\t\t AVAILABLE CARS\n";
                bool foundCar = false;
                for (auto i: car_ids) {
                    if (!car[i].is_rented) {
                        car[i].view_to_user(false);
                        foundCar = true;
                    }
                }
                if (!foundCar) {
                    cout << "\n\t\t (No available cars.)\n";
                    cout<<" Press any key to go back ";
                    string x;
                    cin>>x;
                    system("clear");
                    customer_dashboard();
                } 
                else {
                    string x;
                    cout<<"\n\t Enter the number of days for which you wish to rent:";
                    cin>>x;
                    try {
                        int days= stoi(x);
                        cout << "\n\t Enter ID of the Car you wish to rent:";
                        cin >> car_id;
                        if (car.find(stoi(car_id)) == car.end()) {
                            cout << "\n\t\t No Car with this ID exists!\n";
                            cout<<"\n Press any key to go back\n";
                            cin>>x;
                            customer_dashboard();
                        } 
                        else if(car[stoi(car_id)].is_rented){
                            car[stoi(car_id)].view_to_manager();
                            cout<<"\n\t\t The car is already rented!\n";
                            cout<<"\n Press any key to go back\n";
                            string x;
                            cin>>x;
                            customer_dashboard();
                        }
                        else {
                            rent_car(stoi(car_id), days);
                            string x;
                            cout<<"press any key to go back";
                            cin>>x;
                            customer_dashboard();
                        }
                    }
                    catch(...){
                        cout<<"\n invalid input!\n";
                        customer_dashboard();
                    }
                }
            } 
            else if (op == "2") {
                for(auto c: rented_cars){
                    car[c].view_to_user(false);
                }
                cout << "\n\t Enter Car ID to be returned:";
                cin >> car_id;
                bool present=false;
                for(auto it = rented_cars.begin(); it != rented_cars.end(); ++it){
                    if(*it==stoi(car_id)){
                        present=true;
                        rented_cars.erase(it);
                    } 
                }
                cout<<car[id].id<<endl;
                if(present){
                    rented_num--;
                    car[stoi(car_id)].is_rented=false;
                    car[stoi(car_id)].rented_to.first=-1;
                    cout<<"car with id:"<<car_id<<"returned \n";
                } 
                else{
                    cout<<"\n You have not rented car with id: "<<car_id<<endl;
                    cout<<" Press any key to go back\n";
                    string x;
                    cin>>x;
                    customer_dashboard();
                }
                string x;
                cout<<"\n\t\t How many days has it been, after you rented this car?:";
                cin>>x;
                try{
                    int days_passed= stoi(x);
                    int fine= (max(0,days_passed - car[stoi(car_id)].days))*100;
                    if(fine){
                        cout<<"\n\t\t A fine of Rs "<<fine<<" has to be paid\n";
                        customer_record=max(0,(customer_record-1));
                        cout<<"\n\t\t The maximum number of cars you can rent now is:"<< customer_record<<endl;
                        // emp can now max rent these many cars
                    }
                    else{
                        cout<<"\n\t\t No fine is applied!\n";
                        customer_record=min(10,(customer_record+1));
                        // emp can now rent a max of these many cars (reward for returning on time)
                    }
                }
                catch(...){
                    cout<<"\n Invalid Input!\n";
                    customer_dashboard();
                }
                cout<<" Press any key to go back\b";
                cin>>x;      
                system("clear");
                customer_dashboard();        
            } 
            else if (op == "3") {
                cout << "\n\t\t Number of cars that can be rented:" << (customer_record - rented_num) << endl;
                
                for(auto c: rented_cars){
                    car[c].view_to_user(false);
                }
                cout<<" Press any key to go back\n";
                string x;
                cin>>x;
                system("clear");
                customer_dashboard();
                // customer[ID].view();
            } 
            else if (op == "4") {
                return;
            }

        }

        void Register(){
            cout<<"\t\t Your ID is: "<<id<<endl;
            cout<<"\t\t Customer Registered"<<endl;
            cout<<"\t\t press any key to go back \n";
            string x;
            cin>>x;
        }
        void Login(){
            string password;
            cout<<"Enter Password:";
            cin>>password;
            if(this->password==password){
                system("clear");
                cout<<"\n Welcome! (the customer is logged in)\n";
                customer_dashboard();
            }
            else{
                cout<<"\n\t\tWrong Password!\n";
                cout<<"Press any key to go back \n";
                string x;
                cin>>x;
            }
        }
};

map<int,Employee> employee;
map<int,Customer> customer;


class Manager{
    private:
        string password;
    public:
        Manager(string password="CS253"){
            this->password=password;
        }

        void Edit_car(){
            system("clear");
            cout<<"\n\t\t 1. Add car\n";
            cout<<"\n\t\t 2. Remove car\n";
            cout<<"\n\t\t 3. Update car\n";
            cout<<"\n\t\t 4. Back\n";
            string op1, car_id;
            cin >> op1;
            if (op1 == "1") {
                string model, cost, condition;
                cout << "\n Enter Car Model:";
                cin >> model;
                cout << "\n Enter Car Cost:";
                cin >> cost;
                cout << "\n Enter Car Condition:";
                cin >> condition;
                // carID, model, condition, cost, isrented, pair(rented_to_Id, rented_to_customer(false)/employee(true))
                try{
                    Cars new_car(*available_car_ids.begin(), model, stoi(condition), stoi(cost));
                    car_num++;
                    available_car_ids.erase(available_car_ids.begin());
                    car[new_car.id] = new_car;
                    car_ids.insert(new_car.id);
                    cout<<"\t\n Car added\n";
                }
                catch(...){
                    cout<<" Invalid Inputs!\n";
                    cout<<" Press any key to go back\n";
                    string x;
                    cin>>x;
                    Edit_car();
                }
                cout<<"press some key:";
                string x;
                cin>>x;
            } 
            else if (op1 == "2") {
                cout << "\n Enter Car ID (to be removed)(press b to go back):";
                cin >> car_id;
                if(car_id=="b") Edit_car();
                if(car.find(stoi(car_id)) != car.end()){
                    bool is=car[stoi(car_id)].is_rented;
                    bool isEmp=car[stoi(car_id)].rented_to.second;
                    if(is){
                        if(isEmp){
                            employee[car[stoi(car_id)].rented_to.first].rented_cars.erase(remove(employee[car[stoi(car_id)].rented_to.first].rented_cars.begin(), employee[car[stoi(car_id)].rented_to.first].rented_cars.end(), stoi(car_id)), employee[car[stoi(car_id)].rented_to.first].rented_cars.end());
                            employee[car[stoi(car_id)].rented_to.first].rented_num--;
                        }
                        else{
                            customer[car[stoi(car_id)].rented_to.first].rented_cars.erase(remove(customer[car[stoi(car_id)].rented_to.first].rented_cars.begin(), customer[car[stoi(car_id)].rented_to.first].rented_cars.end(), stoi(car_id)), customer[car[stoi(car_id)].rented_to.first].rented_cars.end());
                            customer[car[stoi(car_id)].rented_to.first].rented_num--;
                        }
                    }
                    
                    car_num-=1;
                    car.erase(stoi(car_id));
                    car_ids.erase(stoi(car_id));
                    cout<<"\n\t\t Car with ID: "<<car_id<<" has been removed!\n";
                } 
                else{
                    cout<<"\n No such ID exists!\n";
                }
                cout<<" Press any key to go back\n";
                    string x;
                    cin>>x;
                    Edit_car();
            } 
            else if (op1 == "3") {
                cout << "\n Enter Car ID (to be updated):";
                cin >> car_id;
                if(car.find(stoi(car_id)) != car.end()){
                    car[stoi(car_id)].update();
                }
                else{
                    cout<<"\n\t\t No such ID exists!\n";
                }
                cout<<"Press any key to continue\n";
                string x;
                cin>>x;
                Edit_car();
            } 
            else if (op1 == "4") {
                system("clear");
                car_rental_system();
            } 
            else {
                cout<<"\nEnter a valid value!\n";
                Edit_car();
            }
        }
        void Edit_User(){
            system("clear");
            cout<<"\n\t\t 1. Add User\n";
            cout<<"\n\t\t 2. Remove User\n";
            cout<<"\n\t\t 3. Update User\n";
            cout<<"\n\t\t 4. View User info\n";
            cout<<"\n\t\t 5. Back\n";
            string op1;
            cin >> op1;

            if (op1 == "1") {
                string password, is_employee, name;
                cout<< "\n Enter Name:";
                cin>>name;
                cout << "\n Enter Password:";
                cin >> password;
                while (true) {
                    cout << "\n Is the user an employee?(y/n):";
                    cin >> is_employee;
                    
                    if (is_employee == "y") {
                        Employee new_employee(name,password);
                        employee[*available_emp_ids.begin()] = new_employee;
                        emp_ids.insert(*available_emp_ids.begin());
                        available_emp_ids.erase(available_emp_ids.begin());
                        emp_i++;
                        
                        cout<<"\n\t\t Employee added!\n";
                        cout<<"The ID is:"<<new_employee.id<<endl;
                        cout<<"Press any key to go back\n";
                        string x;
                        cin>>x;
                        break;
                    } 
                    else if (is_employee == "n") {
                        Customer new_customer(name,password);
                        customer[*available_cus_ids.begin()] = new_customer;
                        cus_ids.insert(*available_cus_ids.begin());
                        available_cus_ids.erase(available_cus_ids.begin());
                        cus_i++;

                        cout<<"\n\t\t Customer added!\n";
                        cout<<"The ID is:"<<new_customer.id<<endl;
                        cout<<"Press any key to go back\n";
                        string x;
                        cin>>x;
                        break;
                    } 
                    else {
                        cout << "\nEnter a valid letter!\n";
                    }
                }
            } 
            else if(op1 == "2"){
                cout<<"\n\t\t Do you wish to remove an employee or customer?(e/c):";
                string x, id;
                cin>>x;
                if(x=="e"){
                    cout<<"\n\t\t Enter the employee ID(press b to go back):";
                    cin>>id;
                    if(id=="b") Edit_User();
                    if(employee.find(stoi(id)) == employee.end()){
                        cout<<"\n No such ID exists!\n";
                        cout<<"Press any key to go back\n";
                        cin>>x;
                        Edit_User();
                    }
                    else{
                        for(auto i:employee[stoi(id)].rented_cars){
                            car[i].is_rented=false;
                            car[i].rented_to.first=-1;
                        }
                        employee.erase(stoi(id));
                        emp_i--;
                        cout<<"\n\t\t Employee removed!\n";
                        cout<<"Press any key to go back\n";
                        cin>>x;
                    }
                }
                else if(x=="c"){
                    cout<<"\n\t\t Enter the customer ID:";
                    cin>>id;
                    if(customer.find(stoi(id)) == customer.end()){
                        cout<<"\n No such ID exists!\n";
                        Edit_User();
                    }
                    else{
                        employee.erase(stoi(id));
                        emp_i--;
                        cout<<"\n\t\t Customer removed!\n";
                        cout<<"Press any key to go back\n";
                        string x;
                        cin>>x;
                    }
                }
                else{
                    cout<<"\n Enter a valid input!\n";
                    Edit_User();
                }
            }
            else if(op1 == "3"){
                cout<<"\n\t\t Is the user an employee or customer?(e/c):";
                string x;
                cin>>x;
                if(x == "e"){
                    cout<<"\n\t\t Enter Employee ID:";
                    cin>>x;
                    employee[stoi(x)].update();
                }
                else if(x == "c"){
                    cout<<"\n\t\t Enter Customer ID:";
                    cin>>x;
                    customer[stoi(x)].update();
                }
                else{
                    Edit_User();
                }
            }
            else if(op1 == "4"){
                cout<<"Do you wish to view employee or customer data?(e/c):";
                string x;
                cin>>x;
                if(x=="c"){
                    for(auto i:cus_ids){
                        customer[i].view();
                    }
                }
                else if(x=="e"){
                    for(auto i:emp_ids){
                        employee[i].view();
                    }
                }
                else{
                    cout<<"\nEnter a valid letter!\n";
                    cout<<"\nPress any key to continue";
                    Edit_User();
                }
                cout<<"\nPress any key to go back";
                cin>>x;
                Edit_User();
            }
            else if(op1 == "5"){
                return;
            }
            else{
                Edit_User();
            }

        }
        void Dashboard(){
            cout<<"\n\t\t Welcome! Manager Successfully Logged in\n";
            while(true) {
                system("clear");
                cout<<"\n\t\t MANAGER OPERATIONS:\n";
                cout<<"\t\t 1. Edit Car Database\n";
                cout<<"\t\t 2. Edit User Database\n";
                cout<<"\t\t 3. View Cars Info\n";
                cout<<"\t\t 4. Logout\n\t\t";

                string op;
                cin >> op;

                if (op == "1") {
                    Edit_car();
                    system("clear");
                } 
                else if (op == "2") {
                    Edit_User();
                } 
                else if (op == "3") {
                    for(auto it = car.begin(); it != car.end(); ++it){
                        it->second.view_to_manager();
                    }

                    cout<<"press any key to go back"<<endl;
                    string p;
                    cin>>p;
                } 
                else if (op == "4") {

                    break; 
                } 
                else {
                    cout<<"\n\t\tEnter a valid number!\n";
                }
            }
        }
        void Login(){
            string password;
            cout<<"\nEnter the Manager Password:";
            cin>>password;
            if(password==this->password){
                Dashboard();
            }
            else{
                cout<<"\nWrong Password!\n press any key to go back";
                string x;
                cin>>x;
            }
        }

};


void car_rental_system(){
    system("clear");
    drawline('-');
    cout<<"num of cars:"<<car_num<<" "<<"num of employees"<<emp_i<<" "<<"num of customers:"<<cus_i<<endl;
    cout<<"\n\t\t\t\t CAR RENTAL SYSTEM \n";
    cout<<"\n\t\t 1.Register\n";
    cout<<"\n\t\t 2.Login\n";
    cout<<"\n\t\t 3.Manager Login\n";
    cout<<"\n\t\t 4.Save\n\t\t\t";
    string op;
    string x;
    cin >> op;

    if (op == "1") {
        while (true) {
            string password, name;
            cout << "Are you an Employee?(y/n)(press b to go back):";
            cin >> x;
            if (x == "y") {
                cout<< "\n Enter Name:";
                cin>>name;
                cout << "\nEnter Password:";
                cin >> password;
                Employee new_employee(name, password);
                emp_ids.insert(new_employee.id);
                emp_i++;
                new_employee.Register();
                employee[new_employee.id] = new_employee;
                available_emp_ids.erase(new_employee.id);
                break;
            } 
            else if (x == "n") {
                cout<< "\n Enter Name:";
                cin>>name;
                cout << "\nEnter Password:";
                cin >> password;
                Customer new_customer(name, password);
                cus_ids.insert(new_customer.id);
                cus_i++;
                new_customer.Register();
                customer[new_customer.id] = new_customer;
                available_cus_ids.erase(new_customer.id);
                break;
            } 
            else if (x == "b"){
                system("clear");
                car_rental_system();
            }
            else
                cout << "\nEnter a valid value!\n";
        }
    } 
    else if (op == "2") {
        string id;
        system("clear");
        while (true) {
            cout << "Are you an Employee?(y/n)(press b to go back):";
            cin >> x;
            if (x == "y") {
                cout << "\nEnter ID:";
                cin >> id;
                try{
                    if (employee.find(stoi(id)) == employee.end()) {
                        cout << "\n\t ID doesn't exist \n";
                        continue;
                    } 
                    else {
                        employee[stoi(id)].Login();
                    }
                    break;
                }
                catch(...){
                    cout<<"\n\t\t Enter an Integer!\n";
                    cout<<"\t\t Press any key to go back:";
                    cin>>x;
                    car_rental_system();
                }
            } 
            else if (x == "n") {
                cout << "\nEnter ID:";
                cin >> id;
                if (customer.find(stoi(id)) == customer.end()) {
                    cout << "\n\t ID doesn't exist \n";
                    continue;
                } 
                else {
                    customer[stoi(id)].Login();
                }
                break;
            } 
            else if(x == "b"){
                system("clear");
                car_rental_system();
            }
            else
                cout << "\nEnter a valid value!\n";
                cout<<"\t\t Press any key to go back:";
                cin>>x;
                car_rental_system();
        }
    } 
    else if (op == "3") {
        Manager manager;
        manager.Login();
    } 
    else if(op == "4"){
        write();
        cout<<"\n Press control-C to confirm exit\n";
        usleep(1000);
    }
    else {
        cout<<"\n\t\tEnter a valid number!\n";
    }

    system("clear");
    car_rental_system();
}

void read_cars(){
    fstream fin; 

    fin.open("car_data.csv", ios::in); 

    int carID, cost, condition, userID, days; 
    bool rented;
    // carID, model, condition, cost, isrented, rented_to_Id, rented_to_customer(false)/employee(true)
    // Read the Data from the file 
    // as String Vector 
    vector<string> row; 
    string line, word, temp; 
    bool rented_to_emp;
  
    while (getline(fin, line)) {
        stringstream s(line); 
        vector<string> row;
        string word;
        row.clear();
        while (getline(s, word, ',')) {
            row.push_back(word);
        }

        car_num++;
        carID = stoi(row[0]); 
        condition = stoi(row[2]);
        cost = stoi(row[3]);
        userID = stoi(row[5]);
        days= stoi(row[7]);
        
        if(row[4]=="rented") rented= true;
        else rented=false;
        if(row[6]=="employee") rented_to_emp= true;
        else rented_to_emp=false;

        car_ids.insert(carID);
        available_car_ids.erase(carID);
        Cars new_car(carID, row[1],condition,cost,rented,make_pair(userID,rented_to_emp), days);
        car[carID] = new_car;
    } 
}

void read_customer(){
    fstream fin; 

    // Open an existing file 
    fin.open("customer_data.csv", ios::in); 
  
    // Get the roll number 
    // of which the data is required 
    int cusID, rented_num, fine_due, customer_record; 
    
    // ID, rented_num, rented_cars.., fine_due, customer_record
    // Read the Data from the file 
    // as String Vector 
    vector<string> row; 
    string line, word, temp, password, name; 
  
    while (getline(fin, line)) {
        stringstream s(line); 
        vector<string> row;
        string word;
        row.clear();
        while (getline(s, word, ',')) {
            row.push_back(word);
        }
  
        cus_i++;
        cusID = stoi(row[0]);
        name = row[1]; 
        password = row[2];
        rented_num = stoi(row[3]);
        customer_record = stoi(row[rented_num + 4]);

        Customer cus(name , password, customer_record, rented_num);
        for(int i=0; i<rented_num; i++){
            cus.rented_cars.push_back(stoi(row[4+i]));
        }
        cus_ids.insert(cusID);
        customer[cusID] = cus;
        available_cus_ids.erase(cusID);
    } 
}

void read_employee(){
    fstream fin; 

    // Open an existing file 
    emp_i=0;
    fin.open("employee_data.csv", ios::in); 
  
    // Get the roll number 
    // of which the data is required 
    int empID, rented_num, fine_due, employee_record; 
    
    // ID, rented_num, rented_cars.., fine_due, customer_record
    // Read the Data from the file 
    // as String Vector 
    vector<string> row; 
    string line, word, temp, password, name; 
  
    while (getline(fin, line)) {
        stringstream s(line); 
        vector<string> row;
        string word;
        row.clear();
        while (getline(s, word, ',')) {
            row.push_back(word);
        } 
       
        emp_i++;
        empID = stoi(row[0]); 
        name = row[1];
        password = row[2];
        rented_num = stoi(row[3]);
        employee_record = stoi(row[4 + rented_num]);
        
        Employee emp(name, password, employee_record, rented_num);
        for(int i=0; i<rented_num; i++){
            emp.rented_cars.push_back(stoi(row[4 + i]));
        }
        emp_ids.insert(empID);
        employee[empID] = emp;
        available_emp_ids.erase(empID);
        
    } 
}

void write_cars(){
    ofstream outputFile("car_data.csv", ofstream::trunc);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Write data from vectors to the CSV file
    for (auto i: car_ids) {
        string rent;
        if(car[i].is_rented) rent="rented";
        else rent= "not_rented";
        outputFile << car[i].id << "," << car[i].model << "," << car[i].condition << "," << car[i].cost << "," << rent << "," << car[i].rented_to.first << "," << car[i].rented_to.second << "," << car[i].days << "\n";
    }

    outputFile.close();
}

void write_employee(){
    ofstream outputFile("employee_data.csv", ofstream::trunc);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Write data from vectors to the CSV file
    for (auto i: emp_ids) {
        string rent;
        if(car[i].is_rented) rent="rented";
        else rent= "not_rented";
        outputFile << i<< "," << employee[i].name << "," << employee[i].view_password() << "," << to_string(employee[i].rented_num) << ",";
        for(auto j: employee[i].rented_cars){
            outputFile <<to_string(j)<<",";
        }
        outputFile<<to_string(employee[i].employee_record)<<"\n";
    }

    outputFile.close();
}

void write_customer(){
    ofstream outputFile("customer_data.csv", ofstream::trunc);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Write data from vectors to the CSV file
    for (auto i: cus_ids) {
        string rent;
        if(car[i].is_rented) rent="rented";
        else rent= "not_rented";
        outputFile << i<< "," << customer[i].name << "," << customer[i].view_password() << "," << to_string(customer[i].rented_num) << ",";
        for(auto j: customer[i].rented_cars){
            outputFile <<to_string(j)<<",";
        }
        outputFile<<to_string(customer[i].customer_record)<<"\n";
    }

    outputFile.close();
}

void write(){
    write_cars();
    write_employee();
    write_customer();
}


int main(){
    system("clear");
    for(int i=0; i<500; i++){
        available_car_ids.insert(i);
        available_cus_ids.insert(i);
        available_emp_ids.insert(i);
    }
    
    read_cars();
    
    read_employee();
    
    read_customer();

    
    
    car_rental_system();

    write();

    return 0;
}
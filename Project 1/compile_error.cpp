// Code for Project 1
    // Report survey results
    
    #include <iostream>
    using namespace std;
    
    int main()
    {
        int numberSurveyed;
        //Compilation error 1: No semicolon!
        int preferQuarter
        int preferSemester;
    
        cout << "How many students were surveyed? ";
        cin >> numberSurveyed;
        cout << "How many of them prefer the quarter system? ";
        cin >> preferQuarter;
        cout << "How many of them prefer the semester system? ";
        cin >> preferSemester;
            
        //Compilation error 2: Strings can't multiply numbers!
        String pctLikeQuarter = 100.0 * preferQuarter / numberSurveyed;
        double pctLikeSemester = 100.0 * preferSemester / numberSurveyed;

        cout.setf(ios::fixed);
        cout.precision(1);
    
        //Compilation error 3: gout hurts your feet, it doesn't print anything to terminal!
        gout << endl;
        cout << pctLikeQuarter << "% prefer the quarter system." << endl;
        cout << pctLikeSemester << "% prefer the semester system." << endl;

        if (preferQuarter > preferSemester)
            cout << "More students prefer the quarter system to the semester system." << endl;
        else
            cout << "More students prefer the semester system to the quarter system." << endl;
                
    }

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <regex>
using namespace std;

vector<string> clean_sites(int argc, char** argv){
    
    vector<string> sites;
    for(size_t i=1;i<argc;i++)
        sites.push_back(argv[i]);
    

    for(string s : sites){
        string new_s;
        int pos = s.find("www.");
       if(pos == 0){
            new_s = s.substr(4);
        }else if(pos == std::string::npos){
            //not found
            new_s = "www." + s;
        }else{
            //found elsewhere
            continue;
        }
        
        if(std::find(sites.begin(), sites.end(), new_s) == sites.end())
            sites.push_back(new_s);
    }

    return sites;
}

void remove_duplicate(vector<string> cur, vector<string>& tar){
    
    //find lines that match "*.*.*.* sites"
    vector<string> cur_sites;
    for(size_t i=0;i<cur.size();i++){
        vector<string> res;
        boost::split(res, cur[i], boost::is_any_of(" "));// [](char c){return c==' ';});
        if(res.size() == 2){
            std::regex ip ("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
            std::regex web (".*\..*");
            if(std::regex_match(res[0], ip) && std::regex_match(res[1], web))
                cur_sites.push_back(res[1]);
        }
    }
    
    for(size_t i=0;i<tar.size();i++){

        if(std::find(cur_sites.begin(), cur_sites.end(), tar[i]) != cur_sites.end()){
            //printf("\nfound %s\n\n", tar[i].c_str());
            tar.erase(tar.begin() + (i--));
        }
    }
}

int main(int argc, char** argv){

    vector<string> target_sites = clean_sites(argc, argv);

    printf("******* cleaned *********\n");
    for(size_t i=0;i<target_sites.size();i++)
        cout<<target_sites[i]<<endl;
    printf("******* cleaned *********\n");

    ifstream cur_host;
    cur_host.open("/etc/hosts");

    vector<string> file;
    string line;
    while(getline(cur_host, line)){
        file.push_back(line);
    }

    remove_duplicate(file, target_sites);

    cout<<"current host file: \n";
    for(string s : file)
        cout<<s<<endl;

    cout<<"target sites: \n";
    for(string s : target_sites)
        cout<<s<<endl;

}

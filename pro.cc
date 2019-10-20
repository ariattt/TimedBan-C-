#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <regex>
#include <chrono>
#include <ctime>
#include <thread>         // std::this_thread::sleep_for


using namespace std;

vector<string> clean_sites(int argc, char** argv){
    
    vector<string> sites;
    for(size_t i=2;i<argc;i++)
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
//            string ip_pattern = R"^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$";
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

vector<string> update_content(const vector<string> content, const vector<string> targets){
    vector<string> new_content (content);
    
    new_content.push_back("\n# Added By TimedBan");
    new_content.push_back("# --------------------------------------");
    
    for(string s : targets){
        new_content.push_back("127.0.0.1 " + s);
    }
    
    new_content.push_back("# --------------------------------------");
    new_content.push_back("# End of TimedBan Section\n");

    return new_content;
    
}

void update_hosts(const vector<string> content){
    
    ofstream hosts;
    hosts.open ("/etc/hosts");
    
    for(string s : content){
        hosts << s <<endl;
    }

    hosts.close();
}

uint64_t lock_time = 0;

int main(int argc, char** argv){

    if(argc <= 2){
        cout<<"need to specify lock time and target sites\n";
        return 0;
    }
    
    lock_time = stoul(argv[1]);

    vector<string> target_sites = clean_sites(argc, argv);

    printf("******* cleaned *********\n");
    for(size_t i=0;i<target_sites.size();i++)
        cout<<target_sites[i]<<endl;
    printf("******* cleaned *********\n");

    ifstream cur_host;
    cur_host.open("/etc/hosts");

    vector<string> content;
    string line;
    while(getline(cur_host, line)){
        content.push_back(line);
    }
    cur_host.close();

    remove_duplicate(content, target_sites);

    vector<string> new_content = update_content(content, target_sites);

    cout<<"*** current host file: \n";
    for(string s : content)
        cout<<s<<endl;
    
    cout<<"*** target host file: \n";
    for(string s : new_content)
        cout<<s<<endl;

    cout<<"*** target sites: \n";
    for(string s : target_sites)
        cout<<s<<endl;

    //note chrono is fishy on mac. mac time is 1000 times faster than other PC. count() return ms in mac, but s in others
    auto start = std::chrono::system_clock::now();
    update_hosts(new_content);
    cout<<"??? "<< (std::chrono::system_clock::now() - start).count() <<endl; 
    
    while(((std::chrono::system_clock::now() - start).count())/1000/1000 < lock_time){
        cout<<"updating host\n current time "<< (std::chrono::system_clock::now() - start).count()/1000/1000 <<endl; 
        update_hosts(new_content);    
        std::this_thread::sleep_for (std::chrono::seconds(2));
    }
    update_hosts(content); //should put it into destructor so it's always called

}

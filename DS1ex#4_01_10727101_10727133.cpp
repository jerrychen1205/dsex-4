// 10727101_陳則叡_10727133_鄧陳得 
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sys/time.h>
#include <string>
#include <ctime>
using namespace std;

bool ishaveFile;

struct data{
	int OID ; // 工作編號 
	int Arrival ; // 進入時刻 
	int Duration ; // 工作耗時 
	int TimeOut ; // 多少豪秒
};

struct Outdata{
	int OID ;
	int Abort ;
	int Departure ;
	int Delay ;
};

struct RunData{
	data Data;
	bool isRun;
	int time;
};

long long getTime() {
    struct timeval time;
    gettimeofday( &time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000; // 轉成毫秒
}

class Queue{
	public:
		Queue();
		data dequeue(bool dele,bool mission2);
		void PrintQueue();
		bool enqueue( data List_NTail,bool fullJudge );
		bool isEmpty();
		void mission2( vector<Outdata> &abort, vector<Outdata> &down );
		void mission2_1( vector<Outdata> &abort, vector<Outdata> &down, int i,RunData &rundata,int &remainT);
		long long  ShellSort();
		int WritingData( FILE *file );
	private:
		vector<data> QueueList;
		vector<data> tempQueue;
	
};

Queue::Queue(){
}

void Queue::mission2( vector<Outdata> &abort, vector<Outdata> &down ){ //更新start 
	int remainT = 0;
	RunData rundata ;
	rundata.time = 0;
	rundata.isRun = false;
	data tempData=dequeue(true,false);
	for( int i = 0 ; QueueList.size() > 0 || tempQueue.size() > 0 || rundata.isRun ; i++ ){
		if( rundata.isRun ){
			remainT--;
			if( i >= rundata.Data.TimeOut && remainT != 0 ) {
				Outdata temp;
           	  	temp.OID = rundata.Data.OID;
          	 	temp.Abort = i;
                temp.Delay = i - rundata.Data.Arrival;
                abort.push_back(temp);
                rundata.isRun = false;
				
				mission2_1( abort, down, i,rundata,remainT);
			} else if ( remainT == 0 ){
                rundata.isRun = false;
				Outdata temp;
           	  	temp.OID = rundata.Data.OID;
          	 	temp.Departure = rundata.time + rundata.Data.Duration ;
                temp.Delay = rundata.time - rundata.Data.Arrival;
                down.push_back(temp);
                
				mission2_1( abort, down, i,rundata,remainT);
				
				
			}
		}else
			mission2_1( abort, down, i,rundata,remainT);
		
		while( tempData.Arrival == i ){
			if(!enqueue(tempData,true)){
				Outdata temp;
				temp.OID = tempData.OID;
				temp.Abort = tempData.Arrival;
				temp.Delay = 0;
				abort.push_back(temp);
			}
			
			mission2_1( abort, down, i,rundata,remainT);
				
			if( !isEmpty() )
					tempData = dequeue(true,false);
			else
				break;
		}
		

	
	}
} 

void Queue::mission2_1( vector<Outdata> &abort, vector<Outdata> &down, int i,RunData &rundata,int &remainT){
 	while( ! tempQueue.empty() && !rundata.isRun ){
		rundata.Data = dequeue(true,true);
		Outdata temp;
		if( i >= rundata.Data.TimeOut ){
           	temp.OID = rundata.Data.OID;
          	temp.Abort = i;
           	temp.Delay = i - rundata.Data.Arrival;
     		abort.push_back(temp);
		} else {
			remainT = rundata.Data.Duration;
			rundata.time = i;
			rundata.isRun = true;
		}
	}
}//更新結束 
 
int Queue::WritingData( FILE *nowFile ) {

    long long start = getTime();

    fprintf( nowFile, "OID\tArrival\tDuration\tTimeOut\n" );

    for ( int i = 0 ; i < QueueList.size() ; i++ ) {
        fprintf( nowFile, "%d\t%d\t%d\t%d\n", QueueList.at(i).OID, QueueList.at(i).Arrival, QueueList.at(i).Duration, QueueList.at(i).TimeOut );
	}

    

    long long stop = getTime();
    return stop - start;
}

long long Queue::ShellSort(){
    long long start = getTime();
    int gap = QueueList.size()/2;
    int  temp;
    for ( int i = gap ; i > 0 ; i = i / 2 ){
    	for ( int j = i ; j < QueueList.size() ; ++j ) {
    		int loc = j;
    		data nextItem = QueueList.at(j);
    		for(;loc>=i&&(QueueList.at(loc-i).Arrival>nextItem.Arrival);loc=loc-i){
    			QueueList.at(j) = QueueList.at(loc-i);	
			}
			QueueList.at(loc) = nextItem;
		}
	} 
	for ( int i = gap ; i > 0 ; i = i / 2 ){
    	for ( int j = i ; j < QueueList.size() ; ++j ) {
    		int loc = j;
    		data nextItem = QueueList.at(j);
    		for(;loc>=i&&(QueueList.at(loc-i).Arrival==nextItem.Arrival)&&(QueueList.at(loc-i).OID>nextItem.OID);loc=loc-i){
    			QueueList.at(j) = QueueList.at(loc-i);
    			
			}
			QueueList.at(loc) = nextItem;
    			
		}
	}
	
    long long stop = getTime();
    
    return stop-start;
}


bool Queue::enqueue( data List_NTail,bool fullJudge ){
	if ( fullJudge ) {
		if( tempQueue.size() >=3 )
			return false;
		else{
			tempQueue.push_back( List_NTail );
		}		
	} else
		QueueList.push_back( List_NTail );
	return true;
}
data Queue::dequeue(bool dele,bool mission2){
	if( mission2 ){
   	 	if ( !tempQueue.empty() ) {
      	  data tempdata = tempQueue.front();
      	  if(dele)
	  	      tempQueue.erase(tempQueue.begin());
    	    return tempdata;
   	 } else 
			cout << "quene is empty.\n" ;
		
	} else {
		if ( ! isEmpty() ) {
  	      data tempdata = QueueList.front();
  	      if(dele)
		        QueueList.erase(QueueList.begin());
  	      return tempdata;
  	  } else 
			cout << "quene is empty.\n" ;
	}

}
bool Queue::isEmpty(){
    return QueueList.empty();
}

void Queue::PrintQueue() {
    cout << "\tOID\tArrival\tDuration\tTimeOut\n" ;
    for ( int i = 0 ; i < QueueList.size() ; i ++ ) 
        cout << "(" << i+1 << ")" << "\t" << QueueList.at(i).OID << "\t" << QueueList.at(i).Arrival << "\t" << QueueList.at(i).Duration << "\t" << QueueList.at(i).TimeOut << endl;
}

class missionDeal{
	public:
		missionDeal(int type){
  		queue = new Queue();
		cout << "\nInput a file number (e.g., 401, 402, 403, ...): ";
		cin >> nowFileNum;
  		if(type==1){
	  		mission1(nowFileNum);
		}
	  	else if (type==2)
	  		mission2(nowFileNum);
		}
		void openFile( string filename );
		void closeFile();
		int AccessData();
		int WritingData(Queue *temp);
		void mission1(string name);
		void mission2(string name);
		void Simulation();
	
	private:
		string nowFileNum ;
        FILE *nowFile ;
        Queue *queue ;
};

void missionDeal::mission1(string name){
	long long rd=0,sd=0,wd=0;
	openFile(name);
	if(nowFile!=NULL)
		rd = AccessData();
	else
		return;
	closeFile();
	queue->PrintQueue();
	sd = queue->ShellSort();
	wd = WritingData(queue);
	closeFile();
  	system("pause");
  	cout << endl;
  	printf("Reading data: %d clocks(%.2f).\n",rd,(float)rd);
  	printf("Sorting data: %d clocks(%.2f).\n",sd,(float)sd);
  	printf("Writing data: %d clocks(%.2f).\n",wd,(float)wd);
  	cout << "See sorted"+name+".txt\n";
  				
}
void missionDeal::mission2(string name){
	
	openFile(name);
	
	AccessData();
	closeFile();
	queue->ShellSort();
	name = "output"+name+".txt";
    nowFile = fopen( name.c_str() , "w" );
    Simulation();
	closeFile();
  	cout << endl;
  	cout << "The simulation is running...\n";
  	cout << "See "+name+"\n";
  	ishaveFile = false;
}

void missionDeal::Simulation(){
	float time = 0;
	float rate = 0;
	vector <Outdata> AbortJobs ;
	vector <Outdata> JobsDone ;
	int AbortCount = 1;
	int DoneCount = 1;
	queue->mission2(AbortJobs,JobsDone);
	fprintf(nowFile,"\t[Abort Jobs]\n");
	fprintf(nowFile,"\tOID\tAbort\tDelay\n");
	for(; AbortCount <= AbortJobs.size() ; AbortCount++){
		fprintf(nowFile,"[%d]\t%d\t%d\t%d\n",AbortCount,AbortJobs.at(AbortCount-1).OID,AbortJobs.at(AbortCount-1).Abort,AbortJobs.at(AbortCount-1).Delay);
	}
	fprintf(nowFile,"\t[Jobs Done]\n");
	fprintf(nowFile,"\tOID\tDeparture\tDelay\n");
	for(; DoneCount <= JobsDone.size() ; DoneCount++){
		fprintf(nowFile,"[%d]\t%d\t%d\t%d\n",DoneCount,JobsDone.at(DoneCount-1).OID,JobsDone.at(DoneCount-1).Departure,JobsDone.at(DoneCount-1).Delay);
	}
	for(int i = 0 ; i < AbortJobs.size() ;i++){
		time += AbortJobs.at(i).Delay;
	}
	for(int i = 0 ; i < JobsDone.size() ;i++){
		time += JobsDone.at(i).Delay;
	}
	
	AbortCount--;
	DoneCount--;
	
	fprintf(nowFile,"[Average Delay] %.2f ms\n",(float)time /((float)AbortCount+(float)DoneCount));
	rate = (float)DoneCount * 100 / ((float)AbortCount + (float)DoneCount) ;
	fprintf(nowFile,"[Success Rate]  %.2f %%",(float)rate);
	
}


void missionDeal::openFile( string filename ){
	nowFileNum = filename;
	filename = "input"+nowFileNum+".txt";
    nowFile = fopen(filename.c_str(), "r");

    if ( nowFile == NULL ) 
        cout << endl << " ### "<< filename <<" does not exist! ###" << endl;
    else if ( nowFile != NULL )
  		ishaveFile=true;
}

int missionDeal::AccessData(){
    long long start = getTime();
    char initial,initial2;
    
	while(initial != '\n'){
		fscanf(nowFile,"%c", &initial);
	}
	data temp;
    while ( fscanf(nowFile,"%d",&temp.OID) != EOF ) { 
    	
		fscanf( nowFile, "%d%d%d",&temp.Arrival,&temp.Duration,&temp.TimeOut);
		queue->enqueue(temp,false);
   		initial = initial2;
	}
    long long stop = getTime();
    return stop-start;
}



int missionDeal::WritingData(Queue *temp){
	string tempn = "sorted"+nowFileNum+".txt";
    nowFile = fopen( tempn.c_str() , "w" );
    return temp->WritingData( nowFile );
}

void missionDeal::closeFile(){
	fclose(nowFile);
}

int main(int argc, char *argv[]){
	int OptionInput = -1;
	string str = "**** Simulate FIFO Queues by SQF *****\n";
	str += "* 0. Quit                            *\n";
	str += "* 1. Sort a file                     *\n";
	str += "* 2. Simulate one FIFO queue         *\n";
	str += "**************************************\n";
	str += "Input a command(0, 1, 2): ";
	cout << str ;
	cin >> OptionInput;
	while( OptionInput != 0 ){
		if( OptionInput == 1 )
			missionDeal(1);
		else if(OptionInput == 2 )
			missionDeal(2);
		else
			cout << "Command does not exist!\n";
		
		cout << endl;
		cout << str ;
		cin >> OptionInput;
	}
}

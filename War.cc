#include <iostream>
#include <deque>
#include <string>
using namespace std;

class CardError{};

class Card{
	int rank_;

	const static char* const VALUES;
	const static char* const KINDS;

	static int value_index(char value){
		for(int i=0;i<13;i++){
			if(value==VALUES[i]){
				return i;
			}
		}
		throw CardError();
	}

	static int kind_index(char kind){
		for(int i=0;i<4;i++){
			if(kind==KINDS[i]){
				return i;
			}
		}
		throw CardError();
	}

	static int card_rank(char value,char kind){
		int v_index=value_index(value);
		int k_index=kind_index(kind);

		return 4*v_index+k_index;
	}
public:
	Card(int rank=-1)
	: rank_(rank)
    {}

	Card(char value,char kind)
	:rank_(card_rank(value,kind))
	{}

	int get_rank() const{
		return rank_;
	}

	int get_value() const{
		return rank_/4;
	}

	int get_kind() const{
		return rank_%4;
	}

	void print() const{
		cout<<VALUES[get_value()]<<KINDS[get_kind()];
	}
};

const char* const Card::VALUES="23456789TJQKA";
const char* const Card::KINDS="cdhs";

class WarDeck{
	deque<Card> deck_;
public:
	Card dequeue();
	void enqueue(Card card);
	void enqueue(WarDeck& deck);
	bool empty() const;

	void print() const;

	static WarDeck load_deck();
};

Card WarDeck::dequeue(){
	Card res=deck_.front();
	deck_.pop_front();
	return res;
}

void WarDeck::enqueue(Card card){
	deck_.push_back(card);
}

void WarDeck::enqueue(WarDeck& deck){
	while(!deck.empty()){
		enqueue(deck.dequeue());
	}
}

bool WarDeck::empty() const{
	return deck_.empty();
}

void WarDeck::print() const{
	if(empty()){
		return;
	}
	deque<Card>::const_iterator it=deck_.begin();
	(*it).print();
	while(++it!=deck_.end()){
		cout<<' ';
		(*it).print();
	}
	cout << endl;
}

WarDeck WarDeck::load_deck(){
	WarDeck res;
	string line;
	getline(cin,line);
	if(!cin.good()){
		throw CardError();
	}
	for(int i=0;i<line.size();i++){
		Card card(line[i],line[i+1]);
		res.enqueue(card);
	}
	return res;
}

void going_to_war(WarDeck &d1,WarDeck &d2){
	int steps=0;
	bool in_war=false;

	WarDeck warDeck;

	while(!d1.empty() && !d1.empty() && steps<4096){
		steps++;
		Card c1=d1.dequeue();
		Card c2=d2.dequeue();

		warDeck.enqueue(c1);
		warDeck.enqueue(c2);

		if(in_war){
			in_war=false;
		}else if(c1.get_value()>c2.get_value()){
			d1.enqueue(warDeck);
		}else if(c1.get_value()<c2.get_value()){
			d2.enqueue(warDeck);
		}else {
			in_war=true;
		}
	}

	if(!d1.empty() && d2.empty()){
		cout<<"player 1 wins in"<<steps;
	}else if(!d2.empty() && d1.empty()){
		cout<<"player 2 wins in"<<steps;
	}else if(d1.empty() && d2.empty()){
		cout<<"game tied in"<<steps;
	}else{
		cout<<"too many steps";
	}

}


int main(){
	WarDeck deck1=WarDeck::load_deck();
	WarDeck deck2=WarDeck::load_deck();

	going_to_war(deck1,deck2);

	return 0;
}

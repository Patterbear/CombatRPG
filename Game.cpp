#include "Game.h"
#include <string>
#include <vector>
#include <algorithm>

using std::invalid_argument;
using std::logic_error;
using std::to_string;
using std::move;
using std::find_if;

// -------------------- Object class ----------------------


// Parameterised Constructor (name and value)
// Throws error if value of object is negative
// Sets name and value of object using arguments, and sets owner to null pointer
// COMMENT: This method is used for the constructors of subclasses, which execute additional statements
Object::Object(string name, int value) {
	if(value < 0){
		throw invalid_argument("object value cannot be negative");
	} else {
		name_ = name;
		value_ = value;
		owner_ = nullptr;
	}
}


// Destructor
// Deletes object and removes pointer to owner
Object::~Object() {	
	owner_ = nullptr;
}


// Get function (name)
// Returns the name of the object
string Object::getName() const {
	return name_;
}

// Get function (name)
// Returns the value of the object
int Object::getValue() const {
	return value_;
}


// Print method
// Returns a string representation of the object
string Object::print() const {
	return type_ +", name: " + name_ + ", value: " + to_string(value_);
}


// Use method (VIRTUAL)
// Behaviour is defined in subclass override methods 
void Object::use() {}


// Overloaded output stream redirection operator
// Prints contents of given object to specified output stream
std::ostream& operator<<(std::ostream& os, const Object& o) {
	return os << o.print();
}

// ------------------- Food class ----------------------


// Parameterised Constructor (name and value)
// Uses base class constructor but defines type attribute
// COMMENT: I couldn't find a method for getting the class name that works across all compilers, so I had the derived classes define it in an attribute
Food::Food(string name, int value) : Object(name, value) {
	type_ = "Food";
}


// Destructor
Food::~Food() {}


// Use method  (OVERRIDE)
// Increases the stamina of the owner relative to the value of the food object
// Object is deleted from inventory after usage
void Food::use(){

	// Ensures owner cannot be healed above maximun (100)
	if(owner_->stamina_ + value_ > 100){
		owner_->stamina_ = 100;  
	} else {
		owner_->stamina_ += value_;
	}

	auto it = find_if(owner_->items_.begin(), owner_->items_.end(), [&](unique_ptr<Object>& food){return food->getName() == name_;});
		
	owner_->items_.erase(remove(owner_->items_.begin(), owner_->items_.end(), *it));
}


// ------------------- Weapon class ----------------------


// Parameterised Constructor (name and value)
// Uses base class constructor but defines type attribute
// COMMENT: I couldn't find a method for getting the class name that works across all compilers, so I had the derived classes define it in an attribute
Weapon::Weapon(string name, int value) : Object(name, value) {
	type_ = "Weapon";
}


// Destructor
Weapon::~Weapon() {}


// Use method (OVERRIDE)
// Equips the chosen weapon for the owner
// COMMENT: I chose to use a string for the 'weapon in use' as the name can easily be used to locate the object
void Weapon::use(){
	owner_->weapon_ = name_;
}


// ------------------- Armour class ----------------------


// Parameterised Constructor (name and value)
// Uses base class constructor but defines type attribute
// COMMENT: I couldn't find a method for getting the class name that works across all compilers, so I had the derived classes define it in an attribute
Armour::Armour(string name, int value) : Object(name, value) {
	type_ = "Armour";
}


// Destructor
Armour::~Armour() {}


// Use method (OVERRIDE)
// Equips the chosen armour for the owner, adding it to the list of equipped armour
// COMMENT: I chose to use a vector of strings for the 'armour in use' as the names can easily be used to locate the objects
void Armour::use(){
	owner_->armour_.push_back(name_);
}


// ------------------- Player class ----------------------


// Paramaterised Constructor (name)
// Sets name to given name and sets health and stamina to default values
// Weapon in use is set to 'none'
// COMMENT: This method is used for the constructors of subclasses, which execute additional statements
Player::Player(string name) {
	name_ = name;
	health_ = 100;
	stamina_ = 100;

	weapon_ = "none";
}


// Destructor
// Deletes the player and empties the items vector
Player::~Player() {
	items_.clear();
}


// Get function (name)
// Returns name of the player
string Player::getName() const {
	return name_;
}


// Get function (health)
// Returns the health value of the player
int Player::getHealth() const {
	return health_;
}


// Get function (stamina)
// Returns the stamina value of the player
int Player::getStamina() const {
	return stamina_;
}


// Pickup method (unique pointer to object)
// Adds the given object to the player inventory and sets its owner to the player
// Throws a logic error if the player is dead (0 health)
void Player::pickup(std::unique_ptr<Object> o) {
	if(health_  == 0){
		throw logic_error("player is dead");
	} else {
		o->owner_ = this;
		items_.push_back(move(o));
	}
}


// Get function (items)
// Returns a string representation of player items
// Returns 'none' after the 'List of items:' title if player has no items
string Player::getInventory() const {
	string inv = "List of items:";
	if(items_.size() != 0){
		inv += "\n ";
		for(int i = 0; i < items_.size(); i++){
			inv += items_[i].get()->print() + "\n ";
		}

		 // Removes new line character and space on end
		inv.pop_back();
		inv.pop_back();

	} else {
		inv += " none";
	}
	
	return inv;
}


// Print method (VIRTUAL)
// Returns a string representation of player
// Is overriden by Fighter and Healer print methods
// COMMENT: I made this method return an empty string to prevent compiler warnings
string Player::print() const {
	return "";
}


// Use method (object name)
// Calls the use function of the chosen item from player inventory
// Throws logic error if player is dead (0 health)
// Returns false if the item has not been located in player inventory
// COMMENT: I used the 'end()' element of the vector to check if the 'find_if' found the item
bool Player::use(string name) {
	if(health_ == 0){
		throw logic_error("player is dead");
	} else {
		auto it = find_if(items_.begin(), items_.end(), [name](unique_ptr<Object> &object){return object->getName() == name;});
		if (it != items_.end()){ // checks to see if the item has been found
			it->get()->use();
			return true;
		}
	}
	return false;
}


// Overloaded output stream redirection operator
// Prints string representation of given player to specified output stream
std::ostream& operator<<(std::ostream& os, const Player& p) {
	return os << p.print();
}


// ------------------- Fighter class ----------------------


// Parameterised Constructor (name)
// Uses base class constructor but defines type attribute
// COMMENT: I couldn't find a method for getting the class name that works across all compilers, so I had the derived classes define it in an attribute
Fighter::Fighter(string name) : Player(name) {
	type_ = "Fighter";
}


// Destructor
Fighter::~Fighter() {}


// Attack method (other Player)
// Simulates an attack against the other player
// Returns false if the attacker's stamina is under 10 or if the other player's defence is greater than the attack value
// Throws a logic error if either player is dead (0 health)
// Returns true if attack is successful
bool Fighter::attack(Player& other) {
	if(health_ == 0){
		throw logic_error("Player is dead.");
	} else if (other.getHealth() == 0) {
		throw logic_error("Stop! " + other.getName() + " is already dead.");
	}  else {
		if(stamina_ < 10){
			return false;
		} else {
			stamina_ -= 10; // stamina reduced

			int attack = 10;
			if(weapon_ != "none"){ // if the player has a weapon equipped, calculate the attack value
				auto it = find_if(items_.begin(), items_.end(), [this](unique_ptr<Object> &object){return object->getName() == weapon_;});
				attack += it->get()->getValue();
			}

			int defence = 0;
			for(int i = 0; i < other.armour_.size(); i++){ // calculates the defense value based on equipped armour
				string name = other.armour_[i];
				auto it = find_if(other.items_.begin(), other.items_.end(), [name](unique_ptr<Object> &object){return object->getName() == name;});
				defence += it->get()->getValue();
			}
			
			if(attack > defence){
				other.health_ -= attack - defence; // damage is taken

				// If the other player has health less than 0, sets it to 0
				if(other.health_ < 0) {
					other.health_ = 0;
				}
				return true;
			} else if (defence > attack) { // attack fails as defense is too strong
				return false;
			}
		}
	}
	return false; // included to prevent compilation warnings

}


// Print method (OVERRIDE)
// Returns a string representation of fighter
// Adds 'none' after weapon in use and/or armour in use when either are empty
string Fighter::print() const {
	string player = "Name: " + name_ + "\nType: " + type_ + "\nHealth: " + to_string(health_) + "\nStamina: " + to_string(stamina_) + "\n" + getInventory();
	
	player += "\nWeapon in use: " + weapon_; // if player has no weapon, the 'weapon_' attribute is automatically 'none' so there is no need to add extra logic here

	player += "\nArmour in use: ";
	if(armour_.size() == 0){
		player += "none";
	} else {
		for(int i = 0; i < armour_.size(); i++){
			player += armour_[i] + ", ";
		}
		// Gets rid of final comma and space
		player.pop_back();
		player.pop_back();
	}
	return player;
}


// ------------------- Healer class ----------------------


// Parameterised Constructor (name)
// Uses base class constructor but defines type attribute
// COMMENT: I couldn't find a method for getting the class name that works across all compilers, so I had the derived classes define it in an attribute
Healer::Healer(string name) : Player(name) {
	type_ = "Healer";
}


// Destructor
Healer::~Healer() {}


// Heal function
// Heals target player by increasing their health value
// Returns false if healer does not have enough stamina or the target is already at full health
// Throws logic error if healer or target are dead
bool Healer::heal(Player& other) {
	if(health_ == 0){
		throw logic_error("Player is dead.");
	} else if (other.getHealth() == 0) {
		throw logic_error(other.getName() + " is dead.");
	} else {
		if (stamina_ < 10) {
			return false;
		} else if(other.getHealth() == 100) {
			stamina_ -= 10;
			return false;
		} else {
			stamina_ -= 10;
		}
		other.health_ += 20;

		// Ensures target's health cannot exceed maximum
		if(other.health_ > 100) {
			other.health_ = 100;
		}
		return true;
	}
}


// Print method (OVERRIDE)
// Returns a string representation of healer
// Adds 'none' after armour in use when no armour has been equipped
// Excludes the 'weapon in use' line
string Healer::print() const {
	string player = "Name: " + name_ + "\nType: " + type_ + "\nHealth: " + to_string(health_) + "\nStamina: " + to_string(stamina_) + "\n" + getInventory();

	player += "\nArmour in use: ";
	if(armour_.size() == 0){
		player += "none";
	} else {
		for(int i = 0; i < armour_.size(); i++){
			player += armour_[i] + ", ";
		}
		// Gets rid of final comma and space
		player.pop_back();
		player.pop_back();
	}
	return player;
}

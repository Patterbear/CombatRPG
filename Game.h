#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::string;
using std::vector;
using std::unique_ptr;

class Player; // forward declaration so that Object can refer to Player

// ------------------ Object and its subclasses -------------------

class Object {
public:

	// Default constructor has been removed.

	// Constructor, specifying the name and the value of an object.
	Object(string name, int value);

	// Destructor
	~Object();

	// Return the name of the object.
	string getName() const;

	// Return the value of the object.
	int getValue() const;

	// Return a string giving details of the object, as specified in
	// the assignment webpage.
	string print() const;

private:
	// Use this object.
	// Virtual function as behaviour is defined by subclasses
	virtual void use() = 0;

protected:
	// Pointer to owner of object
	Player* owner_;

	// Object name
	string name_;

	// Object value
	int value_;

	// Object type
	// Used for print method (no platform-independent way of getting class name) 
	string type_;

// Overloaded output stream redirection operator, printing the object to the
// given output stream
friend std::ostream& operator<<(std::ostream& os, const Object& o);

// Friend classes
friend class Player;
};

class Food : public Object {
public:
	// Constructor
	Food(string name, int value);

	// Destructor
	~Food();

private:
	// Override use method
	// The assignment page says the 'use()' method must be private
	// Due to this I have put this method here, instead of in 'public' as the comments say
	void use() override final;
};

class Weapon : public Object {
public:
	// Constructor
	Weapon(string name, int value);

	// Destructor
	~Weapon();

private:
	// Override use method
	// The assignment page says the 'use()' method must be private
	// Due to this I have put this method here, instead of in 'public' as the comments say
	void use() override final;
};

class Armour : public Object {
public:
	// Constructor
	Armour(string name, int value);

	// Destructor
	~Armour();

private:
	// Override use method
	// The assignment page says the 'use()' method must be private
	// Due to this I have put this method here, instead of in 'public' as the comments say
	void use() override final;
};

// ----------------- Player and its subclasses --------------------

class Player {
public:
	// Default constructor has been removed.

	// Constructor, specifying the name of the player
	// Set the health and stamina both to 100
	Player(string name);

	// Destructor
	~Player();

	// Return the name of the player
	string getName() const;

	// Return the current health of the player
	int getHealth() const;

	// Return the current stamina of the player
	int getStamina() const;

	// Add the object pointed to by the unique pointer to the
	// inventory of objects this player carries.
	// As this is a unique pointer, it has to be "moved" into this
	// function using std::move().
	void pickup(std::unique_ptr<Object> obj);

	// Return a string listing all objects this player owns, in
	// the format specified in the assignment page
	string getInventory() const;

	// Return a string that contains all details of a player, as
	// specified in the assignment page
	// Virtual function as behaviour is defined by subclasses
	virtual string print() const = 0;

	// Use the object with the given name. If the player does not
	// own any object of this name, return false and do nothing,
	// otherwise return true.
	bool use(string name);

protected:
	// Player name
	string name_;

	// Player health
	int health_;

	// Player stamina
	int stamina_;

	// Player type
	// Used for print method (no platform-independent way of getting class name)
	string type_;


	// Player equipped weapon
	string weapon_;

	// Player equipped armour
	vector<string> armour_;
	
	// Player items
	vector<unique_ptr<Object>> items_;

// Overloaded stream redirection operator, that prints the contents of
// the player to the given output stream
friend std::ostream& operator<<(std::ostream& os, const Player& p);

// Friend classes
friend class Food;
friend class Weapon;
friend class Armour;
friend class Fighter;
friend class Healer;
};

class Fighter : public Player {
public:
	// Constructor
	Fighter(string name);

	// Destructor
	~Fighter();

	// Attack the specified player, following the rules in the
	// assignment page
	bool attack(Player& other);

	// Override print method
	// Includes both weapon in use and armour in use lines
	string print() const override final;

private:
	// No non-base class member variables

};

class Healer : public Player {
public:
	// Constructor
	Healer(string name);

	// Destructor
	~Healer();

	// Heal the specified player, following the rules in the
	// assignment page
	bool heal(Player& other);

	// Override print method
	// Includes only armour in use line
	string print() const override final;

private:
	// No non-base class member variables

};

#endif /* GAME_H_ */

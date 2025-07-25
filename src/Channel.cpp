#include "../include/Channel.hpp"
#include "../include/Server.hpp"
#include "algorithm"

Channel::Channel() : _name(""),_topic(""),_users(),  _userLimit(-1), _password(""), _operators(), _isInviteOnly(false), _isTopicRestricted(false)
{
}

Channel::Channel(const std::string &name) : _name(name), _topic(""),_users(), _userLimit(-1), _password(""), _operators(), _isInviteOnly(false), _isTopicRestricted(false)
{
}

Channel::Channel(const Channel &copy) : _name(copy._name), _topic(copy._topic),_users(copy._users), _userLimit(copy._userLimit), 
      _password(copy._password), _operators(copy._operators), 
      _isInviteOnly(copy._isInviteOnly), _isTopicRestricted(copy._isTopicRestricted)
{
	
}

Channel::~Channel()
{
}

Channel& Channel::operator=(const Channel &other)
{
	 if (this != &other)
    {
        _name = other._name;
		_topic = other._topic;
        _users = other._users;
        _userLimit = other._userLimit;
        _password = other._password;
        _operators = other._operators;
        _isInviteOnly = other._isInviteOnly;
        _isTopicRestricted = other._isTopicRestricted;
    }
    return *this;
}

//#### GETTERS 
std::string Channel::getName() const
{
	return this->_name;
}

int Channel::getUserLimit(){
	return (size_t) this->_userLimit;
}

std::string Channel::getPassword(){
	return this->_password;
}

bool Channel::getInviteOnly(){
	return this->_isInviteOnly;
}

bool 		Channel::getTopicRestricted(){
	return this->_isTopicRestricted;
}

std::string Channel::getTopic() const
{
	return this->_topic;
}

//#### SETTERS

void Channel::setName(const std::string &name)
{
	this->_name = name;
}

void		Channel::setUserLimit(int limit){
	this->_userLimit = limit;
}

void		Channel::setPassword (std::string pass){
	this->_password = pass;
}

void		Channel::setInviteOnly(bool invite){
	this->_isInviteOnly = invite;
}

void		Channel::setTopicRestricted(bool restriction){
	this->_isTopicRestricted = restriction;
}

void Channel::setTopic(const std::string &topic) {
    _topic = topic;
}

//#### FUNCTIONS

void Channel::addUser(Client *user)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == user)
			return;
	}
	_users.push_back(user);
}

void Channel::removeUser(Client *user)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == user)
		{
			_users.erase(_users.begin() + i);
			break;
		}
	}
}

const std::vector<Client *>& Channel::getUser() const
{
	return this->_users;
}


bool	Channel::isOperator(std::string nick){
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i]->getNickname() == nick)
		 return true;
	}
	return false;
}

std::string	Channel::addOperator(Client *user){
	if (isOperator(user->getNickname())){
		return("User is Already Operator");
	}
	for(size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i]->getFd() == user->getFd()){
			_operators.push_back(user);
			//return(RPL_CHANGEMODE(user.getHostname(), this->_name, "+o", user->getNickname()));
		}
	}
	return("User not in the Channel");
}

std::string Channel::removeOperator(Client *user){
	for (size_t i = 0; i < _operators.size(); i++){
		if (_operators[i]->getFd() == user->getFd())
		{
			_operators.erase(_operators.begin() + i);
			return ("User Not Operator anymore");
		}
	}
	return("User was not Operator");
}

void Channel::sendToAll(const std::string &msg, int senderFd, Server &server)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i]->getFd() != senderFd)
		{
			server.SendResponse(_users[i]->getFd(), msg);
		}
	}
}

bool 		Channel::is_channel_empty()
{
	if (_users.empty())
		return true;
	return false;
}

bool Channel::isUserInChannel(const std::string &nickname)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getNickname() == nickname)
            return true;
    }
    return false;
}
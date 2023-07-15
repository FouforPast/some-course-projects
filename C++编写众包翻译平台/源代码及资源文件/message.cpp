#include"message_ui.h"
#include"database.h"
#include "message.h"
#include<QHash>
#include<QString>
#include"user.h"
#include"widget.h"

message::~message()
{

}

void message::change_state()
{
    m_state = read;
}

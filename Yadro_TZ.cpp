#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//время (часы, минуты)
struct Time {
    int hours, minutes;
};

//Проверка корректности введённого имени клиента
bool isValidClientName(const std::string& name) {
    // Проверить длину имени
    if (name.length() < 1 || name.length() > 20) {
        return false;
    }

    // Проверить, начинается ли имя с буквы
    if (!isalpha(name[0])) {
        return false;
    }

    // Проверить, содержит ли имя только допустимые символы
    for (char c : name) {
        if (!isalnum(c) && c != '_' && c != '-') {
            return false;
        }
    }

    // Имя прошло все проверки
    return true;
}

//Проверка корректности введённого времени
bool isValidTime(const std::string& time) {
    // Проверить длину времени
    if (time.length() != 5) {
        return false;
    }

    // Проверить, является ли разделителем двоеточие
    if (time[2] != ':') {
        return false;
    }

    // Проверить часы
    int hours = stoi(time.substr(0, 2));
    if (hours < 0 || hours > 23) {
        return false;
    }

    // Проверить минуты
    int minutes = stoi(time.substr(3, 2));
    if (minutes < 0 || minutes > 59) {
        return false;
    }

    // Время прошло все проверки
    return true;
}

//Перевод в минуты
int h_in_m(struct Time t) {
    return t.hours * 60 + t.minutes;
}

//проверка, что время клиента в диапазоне
bool Check_Time(Time c_time, Time start_time, Time stop_time) {
    int t_client = h_in_m(c_time);
    int t_start = h_in_m(start_time);
    int t_stop = h_in_m(stop_time);
    bool flag{ true };
    if (t_client < t_start)
        flag = false;
    else
        flag = true;

    if (!flag)
        return false;

    if (t_client > t_stop)
        flag = false;
    else
        flag = true;

    return flag;
}

//проверка соответствия ID
bool Check_ID(int ID) {
    if (ID == 1 || ID == 2 || ID == 3 || ID == 4)
        return true;
    else
        return false;
}

//Проверка, что номер стола в диапазоне
bool Check_Table(int c_table, int conf_table) {
    if (c_table > conf_table)
        return false;
    else
        return true;
}

//Проверка, свободен ли стол какой-либо
bool Check_Free_Table(std::vector<bool>& is_table_occupied) {

    for (int i = 0; i < is_table_occupied.size(); i++) {
        if (is_table_occupied[i] == true)
            return true;
    }

    return false;

}

//Проверка, находится ли клиент в очереди
bool Check_Queue(std::vector<std::string>& queue_vec, std::string& c_name) {
    for (int i = 0; i < queue_vec.size(); i++) {
        if (queue_vec[i] == c_name) {
            return true;
        }
    }
    return false;
}

//Проверка, находится ли клиент в клубе
bool Check_Inside(std::vector<std::string>& client_vec, std::string& client) {
    for (int i = 0; i < client_vec.size(); i++) {
        if (client_vec[i] == client) {
            return true;
        }
    }
    return false;
}

//Проверка, свободен ли стол
bool Check_Busy(std::vector<bool>& is_table_occupied, int table) {
    if (is_table_occupied[table - 1] == true) { //false - занят, true - свободен
        return true;
    }
    else {
        return false;
    }
}

//Вывод времени
void Output_time(Time c_time) {
    if (c_time.hours < 10)
        std::cout << "0" << c_time.hours << ":";
    else
        std::cout << c_time.hours << ":";
    if (c_time.minutes < 10)
        std::cout << "0" << c_time.minutes;
    else
        std::cout << c_time.minutes;
}

//Вывод события
void Output(Time c_time, int ID, std::string c_name, int c_table) {
    Output_time(c_time);
    std::cout << " " << ID << " " << c_name;
    if (c_table != 0)
        std::cout << " " << c_table << "\n";
    else
        std::cout << "\n";
}

int main() {
    std::ifstream file("configure.txt");

    if (!file.is_open()) {
        std::cerr << "File opening error." << std::endl;
        return 1;
    }

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // Объявление переменных
    std::string line;

    int i{ 0 }, num_of_tables{ 0 }, price_per_hour{ 0 }, c_event{ 0 }, this_time{ 0 }, last_time{ 0 };
    Time start_time, stop_time, c_time;
    std::string c_name;
    std::vector<std::string> client_vec;
    std::vector<std::string> queue_vec;

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //Число столов
    std::string num_of_tables_str;
    std::getline(file, num_of_tables_str);
    num_of_tables = std::stoi(num_of_tables_str);

    std::vector<bool> is_table_occupied(num_of_tables, true);
    std::vector<std::string> in_tables_vec(num_of_tables);

    std::vector<int> s_time(num_of_tables);
    std::vector<int> f_time(num_of_tables);
    std::vector<int> final_time(num_of_tables);
    std::vector<int> final_sum(num_of_tables);
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //Время
    std::string time;
    std::getline(file, time);

    std::size_t pos = time.find(' ');
    std::string start_time_str = time.substr(0, pos);

    std::string stop_time_str = time.substr(pos + 1);

    if (!isValidTime(start_time_str)) {
        std::cout << "Invalid time\n";
        std::cout << start_time_str;
        return -1;
    }
    if (!isValidTime(stop_time_str)) {
        std::cout << "Invalid time\n";
        std::cout << stop_time_str;
        return -1;
    }

    start_time.hours = std::stoi(start_time_str.substr(0, 2));
    start_time.minutes = std::stoi(start_time_str.substr(3, 2));

    stop_time.hours = std::stoi(stop_time_str.substr(0, 2));
    stop_time.minutes = std::stoi(stop_time_str.substr(3, 2));


    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //Цена за час
    std::getline(file, line);
    price_per_hour = std::stoi(line);

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    std::string line1;
    std::string c_table_str;
    int pos1;
    int c_table{ 0 };

    Output_time(start_time);
    std::cout << std::endl;
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    while (std::getline(file, line)) {

        // = = = = = = = = = = = = = = = = = = =
        //Cчитываем время
        pos = line.find(' ');
        line1 = line.substr(0, pos);
        if (!isValidTime(line1)) {
            std::cout << "Invalid time\n";
            std::cout << line1;
            return -1;
        }
        c_time.hours = std::stoi(line1.substr(0, 2));
        c_time.minutes = std::stoi(line1.substr(3, 2));

        // = = = = = = = = = = = = = = = = = = =
        //Проверка, что события происходят последовательно во времени
        this_time = h_in_m(c_time);
        if (this_time < last_time) {
            std::cout << "Events should be consistent in time\n";
            return -1;
        }

        // = = = = = = = = = = = = = = = = = = =
        //Считываем ID
        line1 = line.substr(pos + 1, 1);
        c_event = std::stoi(line1);

        if (!Check_ID(c_event)) {
            std::cout << "Invalid event number [1..4]\n";
            return -1;
        }

        // = = = = = = = = = = = = = = = = = = =
        //Считываем имя
        pos = line.find(' ');
        pos1 = line.find(' ', pos + 1);
        c_table_str.clear();

        std::string line1 = line.substr(pos1 + 1, pos - pos1 - 1);

        pos = line1.find(' ');
        c_name = line1.substr(0, pos);
        pos = line1.find(' ');
        if (pos != std::string::npos) {
            c_table_str = line1.substr(pos + 1);
            c_table = std::stoi(c_table_str);
            if (!Check_Table(c_table, num_of_tables)) {
                std::cout << "Incorrect table number\n";
                return -1;
            }
        }
        else {
            c_table_str.clear();
            c_table = 0;
        }
        // = = = = = = = = = = = = = = = = = = =
        //Проверка корректности введённого имени пользователя
        if (!isValidClientName(c_name)) {
            std::cout << "Invalid user name\n";
            std::cout << c_name;
            return -1;
        }

        // = = = = = = = = = = = = = = = = = = =
        //Запрос
        Output(c_time, c_event, c_name, c_table);
        switch (c_event) {
        case 1:
            // = = = = = = = = = = = = = = = = = = =
            //Проверка времени, относительно открытия
            if (!Check_Time(c_time, start_time, stop_time)) {
                Output(c_time, 13, "NotOpenYet", 0);
                break;
            }

            // = = = = = = = = = = = = = = = = = = =
            //Проверка, что клиент уже внутри
            if (Check_Inside(client_vec, c_name)) {
                Output(c_time, 13, "YouShallNotPass", 0);
                break;
            }

            client_vec.push_back(c_name);
            queue_vec.push_back(c_name);

            break;
        case 2:
            if (c_table == 0) {
                std::cout << "You cannot sit at a table without specifying its number\n";
                break;
            }
            // = = = = = = = = = = = = = = = = = = =
            //Проверка, что клиент не в клубе
            if (!Check_Inside(client_vec, c_name)) {
                Output(c_time, 13, "ClientUnknown", 0);
                break;
            }
            // = = = = = = = = = = = = = = = = = = 
            //Проверка занят ли стол
            if (!Check_Busy(is_table_occupied, c_table)) {
                Output(c_time, 13, "PlaceIsBusy", 0);
                break;
            }

            // = = = = = = = = = = = = = = = = = = 
            // Проверяем, что им был не занят до этого какой-либо стол
            for (i = 0; i < in_tables_vec.size(); i++) {
                if (in_tables_vec[i] == c_name) {
                    is_table_occupied[i] = true;
                    in_tables_vec[i] = "0";
                    for (int j = 0; j < queue_vec.size(); j++) {
                        if (queue_vec[j] == c_name) {
                            queue_vec[j] = "0";
                        }
                    }

                    f_time[i] = c_time.hours * 60 + c_time.minutes;
                    final_time[i] += f_time[i] - s_time[i];

                    if ((f_time[i] - s_time[i]) % 60 != 0) {
                        final_sum[i] += (((f_time[i] - s_time[i]) / 60) + 1) * price_per_hour;
                    }
                    else {
                        final_sum[i] += ((f_time[i] - s_time[i]) / 60) * price_per_hour;
                    }

                    f_time[i] = 0;
                    s_time[i] = 0;
                }
            }

            is_table_occupied[c_table - 1] = false; //занимает стол
            in_tables_vec[c_table - 1] = c_name;

            //Запоминаем, во сколько началась работа с ПК
            s_time[c_table - 1] = c_time.hours * 60 + c_time.minutes;

            //Убираем из очереди
            if (Check_Queue(queue_vec, c_name)) {
                queue_vec.erase(queue_vec.begin());
            }

            break;
        case 3:
            // = = = = = = = = = = = = = = = = = = =
            //Проверка, если в клубе есть свободные столы и клиент ожидает
            if (Check_Free_Table(is_table_occupied)) {
                Output(c_time, 13, "ICanWaitNoLonger!", 0);
                break;
            }
            // = = = = = = = = = = = = = = = = = = =
            //Проверка, если в очереди ожидания клиентов больше, чем общее число столов, то клиент уходит и генерируется событие 11 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if (queue_vec.size() > num_of_tables) {
                for (i = 0; i < queue_vec.size(); i++) {
                    if (queue_vec[i] == c_name) {
                        queue_vec.erase(queue_vec.begin() + i);
                        Output(c_time, 11, c_name, 0);
                        break;
                    }
                }
            }
            break;
        case 4:
            // = = = = = = = = = = = = = = = = = = =
            //Проверка, что клиент в клубе
            if (!Check_Inside(client_vec, c_name)) {
                Output(c_time, 13, "ClientUnknown", 0);
                break;
            }

            //Освобождаем стол
            for (i = 0; i < in_tables_vec.size(); i++) {
                if (in_tables_vec[i] == c_name) {
                    is_table_occupied[i] = true;
                    in_tables_vec[i] = "0"; //удаление
                    for (int j = 0; j < queue_vec.size(); j++) {
                        if (queue_vec[j] == c_name) {
                            queue_vec[j] = "0";
                        }
                    }

                    f_time[i] = c_time.hours * 60 + c_time.minutes;
                    final_time[i] += f_time[i] - s_time[i];

                    if ((f_time[i] - s_time[i]) % 60 != 0) {
                        final_sum[i] += (((f_time[i] - s_time[i]) / 60) + 1) * price_per_hour;
                    }
                    else {
                        final_sum[i] += ((f_time[i] - s_time[i]) / 60) * price_per_hour;
                    }

                    f_time[i] = 0;
                    s_time[i] = 0;
                }
            }

            //Сажаем человека за стол из очереди
            if (queue_vec.size() > 0) {
                for (i = 0; i < is_table_occupied.size(); i++) {
                    if (is_table_occupied[i] == true) {
                        in_tables_vec[i] = queue_vec[0];
                        queue_vec.erase(queue_vec.begin());
                        is_table_occupied[i] = false;
                        Output(c_time, 12, in_tables_vec[i], i + 1);
                        //Запоминаем, во сколько началась работа с ПК
                        s_time[i] = c_time.hours * 60 + c_time.minutes;
                        break;
                    }
                }
            }

            break;
        }
        last_time = h_in_m(c_time);
    }

    //Проверка, что за столом пусто
    for (i = 0; i < is_table_occupied.size(); i++) {
        if (is_table_occupied[i] == false) {
            Output(stop_time, 11, in_tables_vec[i], i + 1);
            is_table_occupied[i] = true;
            in_tables_vec[i] = "0";

            f_time[i] = stop_time.hours * 60 + stop_time.minutes;
            if ((f_time[i] - s_time[i]) % 60 != 0) {
                final_sum[i] += (((f_time[i] - s_time[i]) / 60) + 1) * price_per_hour;
            }
            else {
                final_sum[i] += ((f_time[i] - s_time[i]) / 60) * price_per_hour;
            }
            final_time[i] += f_time[i] - s_time[i];
            f_time[i] = 0;
            s_time[i] = 0;
        }
    }

    //Вывод времени закрытия клуба
    Output_time(stop_time);
    for (i = 0; i < num_of_tables; i++) {
        std::cout << "\n";
        std::cout << i + 1 << " ";
        std::cout << " " << final_sum[i] << " ";
        if ((final_time[i] / 60) < 10)
            std::cout << "0" << (final_time[i] / 60) << ":";
        else
            std::cout << (final_time[i] / 60) << ":";
        if ((final_time[i] % 60) < 10)
            std::cout << "0" << (final_time[i] % 60);
        else
            std::cout << (final_time[i] % 60);
    }

    file.close();

    std::cout << std::endl;

    return 0;
}
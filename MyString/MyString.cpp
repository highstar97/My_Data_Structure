#include <iostream>

int strlen(const char* s);

class MyString{
    char* str;
    int len;
    int memory_capacity;
public:
    MyString(char c);                       // ���� �ϳ��� ����
    MyString(const char* str);              // ���ڿ��� ���� ����
    MyString(const MyString& str);          // ���� ������
    ~MyString();

    MyString& assign(const MyString& arr);  // ������ str�� �Ű����� ������ ����
    MyString& assign(const char* str);

	MyString& append(int count, const char* str);				// str�� count ���� ��ŭ �ڿ� ����
	MyString& append(const char* str);
	MyString& append(const char* str, int pos, int count);		// str.substr(pos,count)�� �ڿ� ����
	MyString& append(const MyString& mystr);

    MyString& insert(int loc, const MyString& str);    // loc�ε������� ���� str ����
    MyString& insert(int loc, const char* str);     
    MyString& insert(int loc, char c);

    MyString& erase(int loc, int num);       // loc�ε����������� num�� ����

	void Set_len(int len) { this->len = len; }
	void Set_memory_capacity(int memory_capacity) { this->memory_capacity = memory_capacity; }
	void Set_str(const char* str) { this->str = new char[this->memory_capacity]; for(int i=0; i<len; i++){this->str[i] = str[i];} }
	
	int Get_len() const { return len; }
	int Get_memory_capacity() const { return memory_capacity; }
	char* Get_str() const { return str; } 

    char at(int i) const;

    int find(int find_from, const MyString& str) const;
    int find(int find_from, const char* str) const;
    int find(int find_from, char c) const;

    int compare(const MyString& str) const;
    int length() const;
    int capacity() const;
    void reserve(int size);                 // �Ҵ��� �޸��� ũ�⸦ ����

	MyString& operator+(const char* str);
	MyString& operator+(const MyString& mystr);

	char& operator[](const int index) { return str[index]; }
	
	friend std::ostream& operator<<(std::ostream& o, const MyString ms);
};

MyString::MyString(char c){
    len = 1;
    memory_capacity = 1;
    str = new char[1];
    str[0] = c;
}

MyString::MyString(const char* str){
    len = strlen(str);
    memory_capacity = len;
    this->str = new char[len];
    for(int i=0; i<len; i++)
        this->str[i] = str[i];
}

MyString::MyString(const MyString& str){
    len = str.len;
    memory_capacity = str.memory_capacity;      // ���� ����ϱ� memory_capacity ��ŭ ��ƾ��ϴ°� �ƴұ�?
    this->str = new char[memory_capacity];
    for(int i=0; i<len; i++)
        this->str[i] = str.str[i];
}

MyString::~MyString(){ delete[] str; }

MyString& MyString::assign(const MyString& str){
    if(str.len > memory_capacity){
        delete[] this->str;

        this->str = new char[str.len];
        memory_capacity = str.len;
    }
    for(int i=0; i<str.len; i++)
        this->str[i] = str.str[i];
    len = str.len;

    return *this;
}

MyString& MyString::assign(const char* str){
    int str_len = strlen(str);
    if(str_len > memory_capacity){
        delete[] this->str;

        this->str = new char[str_len];
        memory_capacity = str_len;
    }
    for(int i=0; i<str_len; i++){
        this->str[i] = str[i];
    }
    len = str_len;

    return *this;
}

MyString& MyString::append(int count, const char* str){
	int len = strlen(str);
	while(this->len + len*count > this->memory_capacity){
		this->memory_capacity *= 2;
		if(this->len + len*count <= this->memory_capacity){
			char* prev_str = this->str;
			this->str = new char[this->memory_capacity];
			for(int i=0; i<this->len; i++)
				this->str[i] = prev_str[i];
			for(int i=0; i<count; i++){
				for(int j=0; j<len; j++)
					this->str[this->len + (i*len) + j] = str[j];
			}
			delete[] prev_str;

			this->len += len*count;
			return *this;
		}
	}
	for(int i=0; i<count; i++){
		for(int j=0; j<len; j++)
			this->str[this->len + (i*len) + j] = str[j];
	}
	
	this->len += len*count;
	return *this;
}

MyString& MyString::append(const char* str){
	int len = strlen(str);
	while(this->len + len > this->memory_capacity){
		this->memory_capacity *= 2;
		if(this->len + len <= this->memory_capacity){
			char* prev_str = this->str;
			this->str = new char[this->memory_capacity];
			for(int i=0; i<this->len; i++)
				this->str[i] = prev_str[i];
			for(int i=0; i<len; i++)
				this->str[this->len + i] = str[i];
			delete[] prev_str;

			this->len += len;
			return *this;
		}
	}
	for(int i=0; i<this->len; i++)
		this->str[this->len + i] = str[i];
	
	this->len += len;
	return *this;
}

MyString& MyString::append(const char* str, int pos, int count){
	int len;
	if(pos + count <= strlen(str))
		len = count;
	else
		len = count - (pos + count - strlen(str));
	while(this->len + len > this->memory_capacity){
		this->memory_capacity *= 2;
		if(this->len + len <= this->memory_capacity){
			char* prev_str = this->str;
			this->str = new char[this->memory_capacity];
			for(int i=0; i<this->len; i++)
				this->str[i] = prev_str[i];
			for(int i=0; i<len; i++){
				this->str[this->len + i] = str[pos + i];
			}
			delete[] prev_str;

			this->len += len;
			return *this;
		}
	}
	for(int i=0; i<len; i++)
		this->str[this->len + i] = str[pos + i];
	
	this->len += len;
	return *this;
}

MyString& MyString::append(const MyString& mystr){
	int len = mystr.len;
	while(this->len + len > this->memory_capacity){
		this->memory_capacity *= 2;
		if(this->len + len <= this->memory_capacity){
			char* prev_str = this->str;
			this->str = new char[this->memory_capacity];
			for(int i=0; i<this->len; i++)
				this->str[i] = prev_str[i];
			for(int i=0; i<len; i++){
				this->str[this->len + i] = mystr.str[i];
			}
			delete[] prev_str;

			this->len += len;
			return *this;
		}
	}
	for(int i=0; i<len; i++)
		this->str[this->len + i] = mystr.str[i];
	
	this->len += len;
	return *this;
}

MyString& MyString::insert(int loc, const MyString& str){
    if(loc < 0 || loc > len)
        return *this;
    if(len + str.len > memory_capacity){
        if(memory_capacity*2 > len + str.len)
            memory_capacity *= 2;
        else
            memory_capacity = len + str.len;    // 2�躸�� ũ�ٸ� ū ��ŭ �Ҵ�

        char* prev_str = this->str;
        this->str = new char[memory_capacity];

        for(int i=0; i<loc; i++)
            this->str[i] = prev_str[i];
        
        for(int i=0; i<str.len; i++)
            this->str[loc+i] = str.str[i];
        
        for(int i=0; i<len-loc; i++)
            this->str[loc+str.len+i] = prev_str[loc+i];

        delete[] prev_str;

        len = len + str.len;
        return *this;
    }
    for(int i = len-1; i >= loc; i--)
        this->str[i+str.len] = this->str[i];
    for(int i=0; i<str.len; i++)
        this->str[i+loc] = str.str[i];
    len += str.len;

    return *this;
}

MyString& MyString::insert(int loc, const char* str){
    MyString temp(str);
    return insert(loc, temp);
}

MyString& MyString::insert(int loc, char c){
    MyString temp(c);
    return insert(loc, temp);
}

MyString& MyString::erase(int loc, int num){
    if(num < 0 || loc < 0 || loc >= len)
        return *this;
    int count = 0;
    for(int i=loc+num; i<len; i++){
        count++;
        str[i-num] = str[i];
    }
    len -= count;
    return *this;
}

char MyString::at(int i) const{
    if(i >= len || i < 0)
        return NULL;
    else
        return str[i];
}

int MyString::find(int find_from, const MyString& str) const{
    if(str.len == 0)
        return -1;
    
    int j;
    for(int i=find_from; i<=len - str.len; i++){
        for(j=0; j<str.len; j++){
            if(this->str[i+j] != str.str[j])
                break;
        }

        if(j == str.len)
            return i;
    }

    return -1;
}

int MyString::find(int find_from, const char* str) const{
    MyString temp(str);
    return find(find_from, temp);
}

int MyString::find(int find_from, char c) const{
    MyString temp(str);
    return find(find_from, temp);
}

int MyString::compare(const MyString& str) const{
    // return 1 : *this �� ���������� �ڿ� �´�.
    // return 0 : *this �� str�� ����
    // return -1 : *this �� ���������� �տ� �´�.
    for(int i=0; i<std::min(len,str.len); i++){
        if(this->str[i] > str.str[i])
            return 1;
        else if(this->str[i] < str.str[i])
            return -1;
    }

    if(len == str.len)  
        return 0;
    // abc > abcd
    else if(len > str.len)
        return 1;
    else 
        return -1;
}

int MyString::length() const { return len; }

int MyString::capacity() const { return memory_capacity; }

void MyString::reserve(int size){
    if(size > memory_capacity){
        char* prev_str = str;
        str = new char[size];
        memory_capacity = size;

        for(int i=0; i<len; i++)
            str[i] = prev_str[i];
        
        delete[] prev_str;
    }
}

MyString& MyString::operator+(const char* str){
	int len = strlen(str);
	while(this->len + len > this->memory_capacity){
		this->memory_capacity *= 2;
		if(this->len + len <= this->memory_capacity){
			char* prev_str = this->str;
			this->str = new char[this->memory_capacity];
			for(int i=0; i<this->len; i++){
				this->str[i] = prev_str[i];
			}
			for(int i=0; i<len; i++){
				this->str[this->len + i] = str[i];
			}
			delete[] prev_str;
			
			this->len += len;
			return *this;
		}
	}
	for(int i=0; i<len; i++)
		this->str[this->len + i] = str[i]; 

	this->len += len;
	return *this;
}

MyString& MyString::operator+(const MyString& mystr){
	while(this->len + mystr.len > this->memory_capacity){
		this->memory_capacity *= 2;
		if(this->len + mystr.len <= this->memory_capacity){
			char* prev_str = this->str;
			this->str = new char[memory_capacity];
			for(int i=0; i<this->len; i++)
				this->str[i] = prev_str[i];
			for(int i=0; i<mystr.len; i++)
				this->str[this->len + i] = mystr.str[i];
			delete[] prev_str;

			this->len += mystr.len;
			return *this;
		}
	}
	for(int i=0; i<mystr.len; i++)
		this->str[this->len + i] = mystr.str[i];
	
	this->len += mystr.len;
	return *this;
}

std::ostream& operator<<(std::ostream& o, const MyString ms){
	for(int i=0; i < ms.len; i++){
		std::cout << ms.str[i];
	}
	return o;

}

// �Ϲ� �Լ�
int strlen(const char* s){
	int i=0;
	while(s[i] != '\0'){
		i++;
	}
	return i;
}

char* strcpy(MyString dst, const char* src){
	int len = strlen(src);
	dst.Set_len(len);
	dst.Set_memory_capacity(len);
	dst.Set_str(src);
	return dst.Get_str();
}

char* strcpy(MyString dst, MyString src){
	dst.Set_len(src.Get_len());
	dst.Set_memory_capacity(src.Get_memory_capacity());
	dst.Set_str(src.Get_str());
	return dst.Get_str();
}

int main(){
	MyString ms1('c');
    MyString ms2("hello");
    MyString ms3(ms2);
	std::cout << "ms1 : " << ms1 << std::endl;
	std::cout << "ms2 : " << ms2 << std::endl;
	std::cout << "ms3 : " << ms3 << std::endl;
}
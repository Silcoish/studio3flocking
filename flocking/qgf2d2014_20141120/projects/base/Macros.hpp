#ifndef MACROS_H
#define MACROS_H

#define DEG2RAD 0.0174532925f
//#define PI 3.14159265359	//already exists in other places
#define TAU 6.28318530718

//macro to make plain data members publicly get and set
#define PUBLIC_DATA_PROPERTY(type, var_name) \
	protected: \
		type m##var_name; \
	public: \
		const type& Get##var_name() const  {return m##var_name;} \
		void Set##var_name(const type& v) {m##var_name = v;}

//macro to make plain ptr members publicly get and set
#define PUBLIC_PTR_PROPERTY(type, var_name) \
	protected: \
		type* m##var_name = nullptr; \
	public: \
		type* Get##var_name() const  {return m##var_name;} \
		void Set##var_name(type* v) {m##var_name = v;}

//macro to make plain data members publicly get and set
#define RO_DATA_PROPERTY(type, var_name) \
	protected: \
		type m##var_name; \
	public: \
		const type& Get##var_name() const  {return m##var_name;} 

//macro to make plain ptr members publicly get and set
#define RO_PTR_PROPERTY(type, var_name) \
	protected: \
		type* m##var_name = nullptr; \
	public: \
		type* Get##var_name() const  {return m##var_name;} 

//macro to delete a normal pointer and set null
#define DELETE_NULLIFY(ptr) delete ptr; ptr = nullptr;

#endif//MACROS_H
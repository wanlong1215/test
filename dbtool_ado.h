#if !defined(ADO_WRAPPER_DBTOOL_API_H)
#define ADO_WRAPPER_DBTOOL_API_H
/**
 * ADO_WRAPPER ADO�ӿڵİ�װ�⣬�ṩһ�����򵥵ġ�����C++�����Ա�Ľӿڣ���ADO��ϸ�ں͸���������������
 * ͬʱ����BusRobot��Ŀ�ṩһ���򵥵ġ����������ļ���(���ݿ�)���ӹ����ܡ�
 *
 * �˽ӿ��ṩ3����Connection��PreparedStatement��ResultSet��
 *
 * Connection
 * ʵ�����ӹ���ͬʱ�û�����ʹ���������������
 *
 * PreparedStatement
 * ��װSQL����ִ�нӿڣ����Դ���̬SQL��䣬ͨ��C++���͵Ĳ���ֱ�Ӵ������ݡ�
 *
 * ResultSet
 * SQL��䷵�صĽ���������Է����(��C++����)��ȡÿ�����ݡ�
 *
 * @author anhu.xie 2008-11-06
 */
// ԭ�ƻ�ʵ�ֲ�������һ��DLL�����������º궨��
#ifdef DBTOOL_EXPORTS
#define DBTOOL_API __declspec(dllexport)
#else
#define DBTOOL_API __declspec(dllimport)
#endif
// Ŀǰ����DLL����Դ�뼶������ֱ�����ô�.h�ļ���
// BTW����ҪConnection::Connection(void)�����dbTool.cpp�ŵ��Լ��Ĺ��̡�
#undef DBTOOL_API
#define DBTOOL_API

#include <string>
#include <map>
#import "msado15.dll" rename("EOF", "AdoEOF")
#include <oledberr.h>

// �ӿڲ���
namespace ADO_WRAPPER {

	class ResultSet;
	class PreparedStatement;
	class Connection;
	using std::string;

	/**
	 * ���ݿ�����
	 */
	class DBTOOL_API Connection {
		friend PreparedStatement;
		ADODB::_ConnectionPtr conn_ado; // ����װ��ADO����
		bool created_new_conn; // �����Ƿ��������Լ������ģ�
		void CreateConnection(const char *conn_str, const char *usr, const char *passwd);
	public:
		/**
		 * ���������ļ�����Ĭ�����ݿ�����
		 * �Ȳ��ṩ��
		 */
//		Connection();
		/**
		 * ʹ��ָ�������Ӳ����������ݿ�����
		 * @param conn_str �����ַ���
		 * @param usr ��¼���ݿ���û���
		 * @param passwd ��¼���ݿ������
		 */
		Connection(const char *conn_str, const char *usr, const char *passwd);
		/**
		 * ʹ�����е�ADO Connection����������ݿ�����
		 * @param pconn ���е�ADO���ӣ���Ȼ��һ��IDispatchָ�룬��������ADO Connection����
		 */
		Connection(IDispatch * pconn);
		/**
		 * �������캯����׷�������Լ��򿪵����ӣ�����֤�ر�
		 */
		Connection(const Connection &src);
		/**
		 * ��������������ر������Լ��򿪵�����
		 */
		~Connection();
		/**
		 * ��һ���µ����ݿ�����
		 * @param conn_str ADO�����ַ���
		 * @param usr ��¼���ݿ��õ��û���
		 * @param passwd ��¼���ݿ��õ�����
		 */
		void open(const char *conn_str, const char *usr, const char *passwd) {
			conn_ado = NULL;
			conn_ado.CreateInstance(__uuidof(ADODB::Connection));
			CreateConnection(conn_str, usr, passwd);
		}
		/**
		 * ����һ��ִ��SQL���Ķ���
		 * @return ִ��SQL����PreparedStatement����
		 */
		PreparedStatement CreateStatement();
		/**
		 * ����һ��ִ��SQL���Ķ��󣬲���׼�������SQL���
		 * @param sql_stmt_string Ҫ׼��ִ�е�SQL���
		 * @return ִ��SQL����PreparedStatement���󣬲���������Ѿ�׼���ã�����ֱ�����ò�����������
		 */
		PreparedStatement prepare_statement(const char *sql_stmt_string);
		/**
		 * ���������ʼһ������
		 * @return ����֧�ֶ༶������������򣬷��ص�ǰ����ļ���
		 */
		long begin_trans() { return conn_ado->BeginTrans(); }
		/**
		 * ��������ύ(ȷ��)����
		 * @return ����֧�ֶ༶������������򣬷��ص�ǰ����ļ���
		 */
		long commit_trans() { return conn_ado->CommitTrans(); }
		/**
		 * ���������������
		 * @return ����֧�ֶ༶������������򣬷��ص�ǰ����ļ���
		 */
		long rollback_trans() { return conn_ado->RollbackTrans(); }

		long close() { return conn_ado->Close(); }


		IDispatch* GetDispatch() { return conn_ado; }

		/**
		* ��ȡADO��Error�ӿ�
		* �ýӿڿ��Ի�ȡsql�����룬���_com_error�಻�ܵõ�ADO SQLState������
		*/
		ADODB::ErrorsPtr GetErrors() { return conn_ado->GetErrors(); }
	};

	/**
	 * ִ�����ݿ�SQL�����
	 */
	class DBTOOL_API PreparedStatement {
		Connection conn; // ���ݿ�����
		ADODB::_CommandPtr stmt; // ׼���õ�SQL���
		ADODB::ParametersPtr para; // �����Ҫ���õĲ���
		// (oledb)Provider��֧�ֲ�����Ϣ����ʱ�������ֶ����������ò���
		std::map<long, ADODB::_ParameterPtr> man_para; // �ֶ����õĲ�����
		bool params_refresh; // �Ƿ�֧�ֲ�����Ϣ������
		void set_param_value(long index, _variant_t value, size_t data_len);
	public:
		/**
		 * ���캯��������������
		 * @param conn ���ݿ����Ӷ���ע������ʹ�õ���һ�����󣬶�����һ��ָ��(Ϊ����ǰ�ڴ������)��
		 */
		PreparedStatement(Connection conn);
		/**
		 * ����������������
		 */
		~PreparedStatement();
		/**
		 * ֱ��ִ��(û��Prepare)һ��SQL��䡣
		 * ����û�в�����Ҫ���ã�һ����ִ�е���䣬ʹ�����������
		 * @param sqlString Ҫִ�е���䡣
		 * @return ����ǲ�ѯ(SELECT)��䣬����һ���������
		 * ��������ݸ���(INSERT/UPDATE)��䣬�����ؽ�������붪�����صĶ���
		 */
		ResultSet execute(const char *sqlString);
		/**
		 * ׼��(Prepare)һ��SQL��䡣
		 * ������Ҫ���ò���(��̬SQL)��������Ҫ����ִ�е���䣬�����ȵ���Prepare(const char*)��
		 * Ȼ��(����)����set<Type>(long,<type>)��Execute()ִ��SQL��䡣
		 * @param sqlString Ҫ׼������䡣
		 * @return ����ǲ�ѯ(SELECT)��䣬����һ���������
		 * ��������ݸ���(INSERT/UPDATE)��䣬�����ؽ�������붪�����صĶ���
		 */
		void prepare(const char *sqlString);
		/**
		 * ���ò�����ʱʱ��,�������ã�ADOĬ�ϵ���30s
		 * \param timeout ��ʱֵ����λ��s��0��ʾ���޳���ֱ���������������
		 */
		void set_timeout( long timeout );
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��short���͵�ֵ
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void set_short(long index, short value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��long���͵�ֵ
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void set_long(long index, long value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��bigInt���͵�ֵ
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void set_bigInt(long index, __int64 value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��float���͵ĸ�����
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void set_float(long index, float value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��double���͵ĸ�����
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void set_double(long index, double value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��bool���͵��߼�ֵ
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void set_bool(long index, bool value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ���ַ���
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ(C++���͵��ַ���)
		 */
		void set_string(long index, string value) { set_string(index,value.c_str()); }
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ���ַ���
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ(C���͵��ַ���)
		 */
		void set_string(long index, const char *value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ���ַ���
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ(COM���͵��ַ���)
		 */
		void set_string(long index, const _bstr_t &value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��ʮ������(DECIMAL)
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void setDecimal(long index, const DECIMAL &value);
		/**
		 * ������׼���õĶ�̬SQL���Ĳ���Ϊһ��COM����
		 * @param index ������λ�ã���0��ʼ����
		 * @param value ������ȡֵ
		 */
		void setVariant(long index, const _variant_t &value);
		/**
		 * ������׼���õĶ�̬SQL������ֵΪһ������������(�ֽڴ�)
		 * @param index ������λ�ã���0��ʼ����
		 * @param data �����ֽڴ���λ��
		 * @param data_len ���ݵĳ���(�ֽ���)
		 */
		void set_bytes(long index, const char *data, size_t data_len);
		/**
		 * ������׼���õĶ�̬SQL������ֵΪһ������������(�ֽڴ�)
		 * ����һ��C++���ã��������ù̶����ȵ����ݡ�
		 * @param index ������λ�ã���0��ʼ����
		 * @param data �ַ�����
		 * @tparam data_len ���ݵĳ���(�ֽ���)
		 */
		template<size_t data_len>
		void set_bytes(long index, const unsigned char (&data)[data_len]) {
			set_bytes(index, reinterpret_cast<const char *>(data), data_len);
		}
		/**
		 * ������׼���õĶ�̬SQL��������ֵΪ��(NULL)ֵ
		 * @param index ������λ�ã���0��ʼ����
		 */
		void set_null(long index);
		/**
		 * ִ��һ����׼����(���������˺��ʵĲ���)��SQL��䡣
		 * ������Ҫ���ò���(��̬SQL)��������Ҫ����ִ�е���䣬�����ȵ���Prepare(const char*)��
		 * Ȼ��(����)����set<Type>(long,<type>)��Execute()ִ��SQL��䡣
		 * ���ڶ�̬SQL��䣬�ڵ���Execute()֮ǰ������ÿ�������������á�
		 * @return ����ǲ�ѯ(SELECT)��䣬����һ���������
		 * ��������ݸ���(INSERT/UPDATE)��䣬�����ؽ�������붪�����صĶ���
		 */
		ResultSet execute();
		/**
		* ִ��һ����׼����(���������˺��ʵĲ���)��SQL��䡣
		* �����ؽ����
		*/
		void execute_noRecords();

		ResultSet executeAsync();

		void cancel();

	};

	/**
	 * SQL��ѯ��䷵�صĽ������
	 * ע�����ǽ�ResultSet������(ADO��)RecordSet����Ϊ����ֻ������ȡ��ѯ�Ľ�����������������޸����ݿ����ݡ�
	 */
	class DBTOOL_API ResultSet {
		friend ResultSet PreparedStatement::execute();
		friend ResultSet PreparedStatement::execute(const char *);
		bool lastFieldGetWasNull; // �����ȡ��(�ֶ�)ֵ�Ƿ�ΪNULL
		ADODB::_RecordsetPtr rs; // ����װ��ADO��¼��
		ADODB::FieldsPtr flds; // ���������(�ֶ�)
		/**
		 * ���캯������������
		 * ������Ϊprivate����Ϊ�˽�ֹ�û��Լ����������������ֻ����PreparedStatement���󷵻ء�
		 */
		ResultSet(ADODB::_RecordsetPtr r) : rs(r), flds(r->Fields), lastFieldGetWasNull(true) {}
	public:
		/**
		 * ������������Դ��������
		 */
		~ResultSet() { rs = NULL; flds = NULL; }
		/**
		 * �Ƿ��ѵ����ݼ��ϵ�ĩβ��
		 * @return �������true����ʾ�ѵ������ĩβ��û����Ч�������п��ã�
		 * �������false������һ������(MoveNext()���ߴ򿪽��������)ȡ������Ч�������С�
		 */
		bool db_eof() { return rs->AdoEOF != 0; }
		/**
		 * ����Ƿ��ѵ����ݼ��϶��ˡ�
		 * @return �������true����ʾ�ѵ���������ˣ���һ��MovePrev()û��ȡ����Ч�������У�
		 * �������false����ʶ��һ��MovePrev()����ȡ������Ч��������
		 */
		bool db_bof() { return rs->BOF != 0; }
		/**
		 * �رս������
		 * ����Դ����ĽǶȣ���������ĵ��ú���Ҫ������û��Ѿ�����ʹ��һ���������Ӧ�ùر�����
		 */
		void close() {
			if ( rs->GetState() == ADODB::adStateOpen )
				rs->Close();
		}
		/**
		 * ��������������(��ǰ�����ƶ�)������һ�����ݡ�
		 * move_next()֮��Ӧ�õ���DBEOF()���ж��Ƿ�ȡ����Ч�����ݡ�
		 */
		void move_next() { rs->MoveNext(); }
		/**
		 * ��������������(�Ӻ���ǰ�ƶ�)������һ�����ݡ�
		 * MovePrev()֮��Ӧ�õ���BOF()���ж��Ƿ�ȡ����Ч�����ݡ�
		 */
		void move_prev() { rs->MovePrevious(); }
		/**
		 * ��������������(�Ӻ���ǰ�ƶ�)���ʵ�һ�����ݡ�
		 * MoveFirst()֮��Ӧ�õ���BOF()���ж��Ƿ�ȡ����Ч�����ݡ�
		 */
		void move_first() { rs->MoveFirst(); }
		/**
		 * ��������������(��ǰ�����ƶ�)�������һ�����ݡ�
		 * MoveLastt()֮��Ӧ�õ���DBEOF()���ж��Ƿ�ȡ����Ч�����ݡ�
		 */
		void move_last() { rs->MoveLast(); }
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��Ϊshort���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 */
		short get_short(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��Ϊlong���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		long get_long(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��Ϊbigint���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		__int64 get_bigInt(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��Ϊfloat���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		float get_float(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��Ϊdouble���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		double get_double(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��Ϊbool���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		bool get_bool(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��ΪDECIMAL���͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		DECIMAL getDecimal(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)������ȡ�õ�ֵת��ΪCOM�ַ������͡�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ�����ڿɿյ��У���Ҫ�����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		string get_string(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)��������COM��ͨ�����ͷ��ء�
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @return ȡ�õ���ֵ��
		 * ���ڿɿյ��У����Խ����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		_variant_t getVariant(long index);
		/**
		 * �ӵ�ǰ�л�ȡ�����ֶ�(��ֵ)�����ǰ���ֵ����һ������������(�ֽڴ�)����ȡ��
		 * ע�⣬�ⲻ��ת����������ݿ��ﶨ����в��Ƕ��������ݣ��������쳣��
		 * @param index Ҫ���ʵ����ڽ������λ�ã���0��ʼ������
		 * @param buffer ��Ž�������ڴ���getBytes()���䣬�ɵ����߸����ͷ�(��delete[])��
		 * @return ȡ�õĶ��������ݵ��ֽ�����
		 * ���ڿɿյ��У����Խ����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		size_t get_bytes(long index, char *&buffer);
		/**
		 * ��ȡָ�����ȵĶ���������(�ֽڴ�)
		 * ����һ��C++���ã������ù̶��̶����Ȼ��ȥ������ݡ�
		 * @param index ������λ�ã���0��ʼ����
		 * @param data �ַ�����
		 * @tparam data_len ���ݵĳ���(�ֽ���)
		 */
		template<size_t data_len>
		size_t get_bytes(long index, unsigned char (&data)[data_len]);
		/**
		 * ���һ�ε���get<Type>(long...)�������ص���ֵ�Ƿ�Ϊ��(NULL)ֵ��
		 * ע����wasNull()������isNull()������һ��Ҫ�ȵ���get<Type>(long...)������
		 * @return true��ʶ��󷵻��е�ֵΪNULL��false��ʶ��󷵻���ֵ��Ч��
		 * ���ڿɿյ��У����Խ����ŵ���wasNull()���ж��Ƿ�ΪNULL��
		 */
		bool was_null() { return lastFieldGetWasNull; }
		/**
		 * �����������
		 * @return ���ؽ�������ֶ�(��)����Ŀ��
		 */
		long col_count() { return flds->Count; }
	};
}

//================================================================
// ����Ϊʵ�֡����������¿���;-)
inline ADO_WRAPPER::Connection::Connection(const char *conn_str, const char *usr, const char *passwd)
	: conn_ado("ADODB.Connection"), created_new_conn(false)
{
	CreateConnection(conn_str, usr, passwd);
}
inline ADO_WRAPPER::Connection::Connection(IDispatch * Connection) : created_new_conn(false)
{
	if ( Connection == NULL )
		conn_ado = NULL;
	else {
		HRESULT hr = Connection->QueryInterface(&conn_ado);
		if ( FAILED(hr) )
			_com_issue_error(hr);
		if ( Connection == NULL )
			_com_issue_error(E_INVALIDARG);
	}
}
inline ADO_WRAPPER::Connection::Connection(const Connection &src)
	: conn_ado(src.conn_ado), created_new_conn(false)
{
}
inline ADO_WRAPPER::Connection::~Connection()
{
	if ( created_new_conn )
		conn_ado->Close();
}
inline void ADO_WRAPPER::Connection::CreateConnection(const char *c, const char *u, const char *p)
{
	created_new_conn = false;
	HRESULT hr = conn_ado->Open(c, u, p, 0);
	if ( FAILED(hr) )
		_com_issue_error(hr);
	else
		created_new_conn = true;
	//ʹ�ÿͻ����α�
	//Ĭ������Ƿ��������α꣺adUseServer�����α겻֧��MovePrevious�Ȳ�����
//	hr = conn_ado->put_CursorLocation(ADODB::adUseClient);
// 	if ( FAILED(hr) )
// 		_com_issue_error(hr);
}

//==============
inline ADO_WRAPPER::PreparedStatement ADO_WRAPPER::Connection::CreateStatement()
{
	return PreparedStatement(*this);
}
inline ADO_WRAPPER::PreparedStatement ADO_WRAPPER::Connection::prepare_statement(const char *sqlstr)
{
	PreparedStatement st(*this);
	st.prepare(sqlstr);
	return st;
}

inline ADO_WRAPPER::PreparedStatement::PreparedStatement(Connection dbc)
	: conn(dbc), stmt("ADODB.Command"), params_refresh(false)
{
}
inline ADO_WRAPPER::PreparedStatement::~PreparedStatement()
{
	conn = NULL;
	stmt = NULL;
	para = NULL;
}

inline void ADO_WRAPPER::PreparedStatement::prepare(const char *sql) {
	stmt->ActiveConnection = conn.conn_ado;
	stmt->CommandText = sql;
	stmt->Prepared = true;
	para = stmt->Parameters;
	params_refresh = false;
	man_para.clear();
	if ( para != NULL )
		try {
			para->Refresh();
			params_refresh = true;
	}
	catch ( _com_error &e) {
		if ( e.Error() != DB_E_PARAMUNAVAILABLE )
			throw;
	}
}

inline void ADO_WRAPPER::PreparedStatement::set_param_value(long index, _variant_t value, size_t data_len) {
	if ( params_refresh )
		para->Item[index]->Value = value;
	else {
		ADODB::_ParameterPtr cp = stmt->CreateParameter("", ADODB::adVarBinary, ADODB::adParamInput, data_len, value);
		man_para[index] = cp;
	}
}
inline void ADO_WRAPPER::PreparedStatement::set_timeout(long sec) {
	stmt->PutCommandTimeout( sec );
}

inline void ADO_WRAPPER::PreparedStatement::cancel() {
	
	stmt->Cancel();

}

inline ADO_WRAPPER::ResultSet ADO_WRAPPER::PreparedStatement::executeAsync() {
	if ( !params_refresh )
		for (std::map<long, ADODB::_ParameterPtr>::iterator i = man_para.begin(); i != man_para.end(); ++i )
			para->Append(i->second);
	ADODB::_RecordsetPtr rs = stmt->Execute(NULL,NULL,ADODB::adCmdText | ADODB::adAsyncExecute);
//	return ResultSet(rs);
}

inline void ADO_WRAPPER::PreparedStatement::execute_noRecords() {
	if ( !params_refresh )
		for (std::map<long, ADODB::_ParameterPtr>::iterator i = man_para.begin(); i != man_para.end(); ++i )
			para->Append(i->second);
	stmt->Execute(NULL,NULL,ADODB::adCmdText | ADODB::adExecuteNoRecords);

}
inline ADO_WRAPPER::ResultSet ADO_WRAPPER::PreparedStatement::execute() {
	if ( !params_refresh )
		for (std::map<long, ADODB::_ParameterPtr>::iterator i = man_para.begin(); i != man_para.end(); ++i )
			para->Append(i->second);
	ADODB::_RecordsetPtr rs = stmt->Execute(NULL,NULL,ADODB::adCmdText);
	return ResultSet(rs);
}
inline ADO_WRAPPER::ResultSet ADO_WRAPPER::PreparedStatement::execute(const char *sql) {
	stmt->ActiveConnection = conn.conn_ado;
	stmt->CommandText = sql;
	ADODB::_RecordsetPtr rs = stmt->Execute(NULL,NULL,ADODB::adCmdText);
	return ResultSet(rs);
}
inline void ADO_WRAPPER::PreparedStatement::set_short(long index, short v) {
	set_param_value(index, v, sizeof v);
}
inline void ADO_WRAPPER::PreparedStatement::set_long(long index, long v) {
	set_param_value(index, v, sizeof v);
}
inline void ADO_WRAPPER::PreparedStatement::set_bigInt(long index, __int64 v) {
	// VC < 7����ֱ�Ӵ���__int64:(����DECIMAL
#if _MSC_VER < 0x1300
	DECIMAL dec;
	dec.signscale = 0;
	dec.Hi32 = 0;
	dec.Lo64 = v;
	set_param_value(index, dec, sizeof v);
#else
	set_param_value(index, v, sizeof v);
#endif
}
inline void ADO_WRAPPER::PreparedStatement::set_float(long index, float v) {
	set_param_value(index, v, sizeof v);
}
inline void ADO_WRAPPER::PreparedStatement::set_double(long index, double v) {
	set_param_value(index, v, sizeof v);
}
inline void ADO_WRAPPER::PreparedStatement::set_bool(long index, bool v) {
	set_param_value(index, (v != 0), sizeof v);
}
inline void ADO_WRAPPER::PreparedStatement::set_string(long index, const char *v) {
	set_param_value(index, v, strlen(v));
}
inline void ADO_WRAPPER::PreparedStatement::set_string(long index, const _bstr_t &v) {
	set_param_value(index, v, v.length());
}
inline void ADO_WRAPPER::PreparedStatement::setVariant(long index, const _variant_t &v) {
	if ( v.vt == VT_EMPTY )
		set_null(index);
	else {
		size_t len = 8;
		switch ( v.vt ) {
		case VT_BSTR:
		case VT_VARIANT:
		case VT_LPSTR:
		case VT_LPWSTR:
			len = v.operator _bstr_t().length();
			break;
		case VT_ARRAY:
		case VT_SAFEARRAY:
			len = v.parray->rgsabound[0].cElements;
			break;
		}
		set_param_value(index, v, len);
	}
}
inline void ADO_WRAPPER::PreparedStatement::set_null(long index) {
	_variant_t vNull;
	vNull.vt = VT_NULL;
	set_param_value(index, vNull, 0);
}

inline void ADO_WRAPPER::PreparedStatement::setDecimal(long index, const DECIMAL &v) {
	set_param_value(index, v, sizeof v);
}

inline void ADO_WRAPPER::PreparedStatement::set_bytes(long index, const char *data, size_t data_len) {
	SAFEARRAY *psa = SafeArrayCreateVector(VT_UI1, 0, data_len);
	if ( ! psa )
		_com_issue_error(E_POINTER);
	BYTE *dst = NULL;
	SafeArrayAccessData(psa, (void**)&dst);
	memcpy(dst, data, data_len);
	SafeArrayUnaccessData(psa);

	_variant_t v;
	v.vt = VT_ARRAY | VT_UI1;
	v.parray = psa;
	set_param_value(index, v, data_len);
}


//==================
inline __int64 ADO_WRAPPER::ResultSet::get_bigInt(long index) {
	const _variant_t &v = getVariant(index);
	// VC < 7����ֱ�Ӵ���__int64:(����DECIMAL
#if _MSC_VER < 1300
	return v.operator DECIMAL().Lo64;
#else
	return v;
#endif
}
inline long ADO_WRAPPER::ResultSet::get_long(long index) {
	return getVariant(index);
}
inline short ADO_WRAPPER::ResultSet::get_short(long index) {
	return getVariant(index);
}
inline std::string ADO_WRAPPER::ResultSet::get_string(long index) {
	return (char *)(_bstr_t)getVariant(index);
}
inline double ADO_WRAPPER::ResultSet::get_double(long index) {
	return getVariant(index);
}
inline float ADO_WRAPPER::ResultSet::get_float(long index) {
	return getVariant(index);
}
inline bool ADO_WRAPPER::ResultSet::get_bool(long index) {
	return getVariant(index);
}
inline DECIMAL ADO_WRAPPER::ResultSet::getDecimal(long index) {
	return getVariant(index);
}
inline _variant_t ADO_WRAPPER::ResultSet::getVariant(long index) {
	const _variant_t &v = flds->Item[index]->Value;
	lastFieldGetWasNull = (v.vt == VT_NULL);
	return lastFieldGetWasNull ? _variant_t() : v;
}
inline size_t ADO_WRAPPER::ResultSet::get_bytes(long index, char *&dst) {
	const _variant_t &v = flds->Item[index]->Value;
	lastFieldGetWasNull = (v.vt == VT_NULL);
	if ( v.vt == VT_NULL || v.vt == VT_EMPTY )
		return 0;
	if ( v.vt != (VT_ARRAY | VT_UI1) )
		_com_issue_error(E_INVALIDARG);
	SAFEARRAY *psa = v.parray;
	VARTYPE vt;
	HRESULT hr = SafeArrayGetVartype(psa, &vt);
	if ( FAILED(hr) )
		_com_issue_error(hr);
	if ( vt != VT_UI1 || SafeArrayGetDim(psa) != 1 )
		_com_issue_error(E_INVALIDARG);
	long len = psa->rgsabound[0].cElements;
	dst = new char[len];
	char *src = NULL;
	hr = SafeArrayAccessData(psa, (void**)&src);
	if ( FAILED(hr) )
		_com_issue_error(hr);
	memcpy(dst, src, len);
	SafeArrayUnaccessData(psa);
	return len;
}
template<size_t data_len>
size_t ADO_WRAPPER::ResultSet::get_bytes(long index, unsigned char (&data)[data_len]) {
	const _variant_t &v = flds->Item[index]->Value;
	lastFieldGetWasNull = (v.vt == VT_NULL);
	if ( v.vt == VT_NULL || v.vt == VT_EMPTY )
		return 0;
	if ( v.vt != (VT_ARRAY | VT_UI1) )
		_com_issue_error(E_INVALIDARG);
	SAFEARRAY *psa = v.parray;
	VARTYPE vt;
	HRESULT hr = SafeArrayGetVartype(psa, &vt);
	if ( FAILED(hr) )
		_com_issue_error(hr);
	if ( vt != VT_UI1 || SafeArrayGetDim(psa) != 1 )
		_com_issue_error(E_INVALIDARG);
	long len = psa->rgsabound[0].cElements;
	char *src = NULL;
	hr = SafeArrayAccessData(psa, (void**)&src);
	if ( FAILED(hr) )
		_com_issue_error(hr);
	memmove(data, src, len > data_len ? data_len : len);
	SafeArrayUnaccessData(psa);
	return len;
}

#endif

/*�������ò���(�����������ڱ��⡢���ġ���������������Ϣ��)�鱱�����ƻ���ռ似�����޹�˾���У����ҽ����������ƻ���ռ似�����޹�˾�����ض���;������������Ҫ���Ա��ܡ�
*�κνӴ������ͻ���ʹ�ø����ϵ���Ա�����Ż��ߵ�λ�����뱣�ܺ�������ر��ܹ涨��δ����˾��������ϵ�˵�������ɲ���й¶���������κ�������;��
*�κθ��ˡ����Ż��ߵ�λ���ܸ��ơ������������ϣ�Ҳ���ܽ��з����롢���򹤳̡��ֲ��޸Ļ������κ����ϻ��߲������ϡ�
*�Ͻ�δ����Ȩʹ����Щ���ϻ��߲������ϡ� 
*These materials including, but not limited to data��datum, document, reference, 
*information are owned by Beijing UCAS Space Technology Co.,Ltd, 
*and can only be used for special purpose by Beijing UCAS Space Technology Co.,Ltd. 
*Those materials mentioned above are strictly confidential. 
*Any person, department or entity shall keep them secret and observe the secrecy provisions when contacting,delivering or using these materials. 
*Any of these materials shall not be disclosed or used in other ways without the written permission of Beijing UCAS Space Technology Co.,Ltd and the interested party submitting it. 
*Any person, department or entity may not copy or reproduce those materials mentioned above nor may decompile, reverse engineer, disassemble, modify or create derivative works of any of these materials, or any portion thereof. 
*Any unauthorized use of the materials or any portion thereof is strictly prohibited. */
 
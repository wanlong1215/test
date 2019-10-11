#if !defined(ADO_WRAPPER_DBTOOL_API_H)
#define ADO_WRAPPER_DBTOOL_API_H
/**
 * ADO_WRAPPER ADO½Ó¿ÚµÄ°ü×°¿â£¬Ìá¹©Ò»¸ö¸ü¼òµ¥µÄ¡¢ÃæÏòC++±à³ÌÈËÔ±µÄ½Ó¿Ú£¬°ÑADOµÄÏ¸½ÚºÍ¸´ÔÓÐÔÒþ²ØÆðÀ´£¬
 * Í¬Ê±¶ÔÓÚBusRobotÏîÄ¿Ìá¹©Ò»¸ö¼òµ¥µÄ¡¢»ùÓÚÅäÖÃÎÄ¼þµÄ(Êý¾Ý¿â)Á¬½Ó¹ÜÀí¹¦ÄÜ¡£
 *
 * ´Ë½Ó¿ÚÌá¹©3¸öÀàConnection¡¢PreparedStatement¡¢ResultSet£º
 *
 * Connection
 * ÊµÏÖÁ¬½Ó¹ÜÀí£¬Í¬Ê±ÓÃ»§¿ÉÒÔÊ¹ÓÃËü½øÐÐÊÂÎñ¹ÜÀí¡£
 *
 * PreparedStatement
 * ·â×°SQLÓï¾äµÄÖ´ÐÐ½Ó¿Ú£¬¿ÉÒÔ´¦Àí¶¯Ì¬SQLÓï¾ä£¬Í¨¹ýC++ÀàÐÍµÄ²ÎÊýÖ±½Ó´«µÝÊý¾Ý¡£
 *
 * ResultSet
 * SQLÓï¾ä·µ»ØµÄ½á¹û¼¯£¬¿ÉÒÔ·½±ãµÄ(ÒÔC++ÀàÐÍ)»ñÈ¡Ã¿ÐÐÊý¾Ý¡£
 *
 * @author anhu.xie 2008-11-06
 */
// Ô­¼Æ»®ÊµÏÖ²¿·Ö×ö³ÉÒ»¸öDLL£¬ËùÒÔÓÐÒÔÏÂºê¶¨Òå
#ifdef DBTOOL_EXPORTS
#define DBTOOL_API __declspec(dllexport)
#else
#define DBTOOL_API __declspec(dllimport)
#endif
// Ä¿Ç°²»ÊÇDLL£¬ÊÇÔ´Âë¼¶¹²Ïí£¬¿ÉÖ±½ÓÒýÓÃ´Ë.hÎÄ¼þ¡£
// BTW£¬ÐèÒªConnection::Connection(void)µÄÇë°ÑdbTool.cpp·Åµ½×Ô¼ºµÄ¹¤³Ì¡£
#undef DBTOOL_API
#define DBTOOL_API

#include <string>
#include <map>
#import "msado15.dll" rename("EOF", "AdoEOF")
#include <oledberr.h>

// ½Ó¿Ú²¿·Ö
namespace ADO_WRAPPER {

	class ResultSet;
	class PreparedStatement;
	class Connection;
	using std::string;

	/**
	 * Êý¾Ý¿âÁ¬½Ó
	 */
	class DBTOOL_API Connection {
		friend PreparedStatement;
		ADODB::_ConnectionPtr conn_ado; // Ëù°ü×°µÄADOÁ¬½Ó
		bool created_new_conn; // Á¬½ÓÊÇ·ñÊÇÎÒÃÇ×Ô¼º´´½¨µÄ£¿
		void CreateConnection(const char *conn_str, const char *usr, const char *passwd);
	public:
		/**
		 * ¸ù¾ÝÅäÖÃÎÄ¼þ´´½¨Ä¬ÈÏÊý¾Ý¿âÁ¬½Ó
		 * ÏÈ²»Ìá¹©£¡
		 */
//		Connection();
		/**
		 * Ê¹ÓÃÖ¸¶¨µÄÁ¬½Ó²ÎÊý´´½¨Êý¾Ý¿âÁ¬½Ó
		 * @param conn_str Á¬½Ó×Ö·û´®
		 * @param usr µÇÂ¼Êý¾Ý¿âµÄÓÃ»§Ãû
		 * @param passwd µÇÂ¼Êý¾Ý¿âµÄÃÜÂë
		 */
		Connection(const char *conn_str, const char *usr, const char *passwd);
		/**
		 * Ê¹ÓÃÒÑÓÐµÄADO Connection¶ÔÏó¹ÜÀíÊý¾Ý¿âÁ¬½Ó
		 * @param pconn ÒÑÓÐµÄADOÁ¬½Ó£¬ËäÈ»ÊÇÒ»¸öIDispatchÖ¸Õë£¬µ«±ØÐëÊÇADO Connection¶ÔÏó
		 */
		Connection(IDispatch * pconn);
		/**
		 * ¿½±´¹¹Ôìº¯Êý£¬×·×ÙÎÒÃÇ×Ô¼º´ò¿ªµÄÁ¬½Ó£¬²¢±£Ö¤¹Ø±Õ
		 */
		Connection(const Connection &src);
		/**
		 * Îö¹¹º¯Êý£¬¸ºÔð¹Ø±ÕÎÒÃÇ×Ô¼º´ò¿ªµÄÁ¬½Ó
		 */
		~Connection();
		/**
		 * ´ò¿ªÒ»¸öÐÂµÄÊý¾Ý¿âÁ¬½Ó
		 * @param conn_str ADOÁ¬½Ó×Ö·û´®
		 * @param usr µÇÂ¼Êý¾Ý¿âÓÃµÄÓÃ»§Ãû
		 * @param passwd µÇÂ¼Êý¾Ý¿âÓÃµÄÃÜÂë
		 */
		void open(const char *conn_str, const char *usr, const char *passwd) {
			conn_ado = NULL;
			conn_ado.CreateInstance(__uuidof(ADODB::Connection));
			CreateConnection(conn_str, usr, passwd);
		}
		/**
		 * ´´½¨Ò»¸öÖ´ÐÐSQLÓï¾äµÄ¶ÔÏó
		 * @return Ö´ÐÐSQLÓï¾äµÄPreparedStatement¶ÔÏó
		 */
		PreparedStatement CreateStatement();
		/**
		 * ´´½¨Ò»¸öÖ´ÐÐSQLÓï¾äµÄ¶ÔÏó£¬²¢ÇÒ×¼±¸´«ÈëµÄSQLÓï¾ä
		 * @param sql_stmt_string Òª×¼±¸Ö´ÐÐµÄSQLÓï¾ä
		 * @return Ö´ÐÐSQLÓï¾äµÄPreparedStatement¶ÔÏó£¬²¢ÇÒÆäÓï¾äÒÑ¾­×¼±¸ºÃ£¬¿ÉÒÔÖ±½ÓÉèÖÃ²ÎÊýºÍÔËÐÐÁË
		 */
		PreparedStatement prepare_statement(const char *sql_stmt_string);
		/**
		 * ÊÂÎñ¹ÜÀí£º¿ªÊ¼Ò»¸öÊÂÎñ
		 * @return ¶ÔÓÚÖ§³Ö¶à¼¶ÊÂÎñµÄÇý¶¯³ÌÐò£¬·µ»Øµ±Ç°ÊÂÎñµÄ¼¶±ð
		 */
		long begin_trans() { return conn_ado->BeginTrans(); }
		/**
		 * ÊÂÎñ¹ÜÀí£ºÌá½»(È·ÈÏ)ÊÂÎñ
		 * @return ¶ÔÓÚÖ§³Ö¶à¼¶ÊÂÎñµÄÇý¶¯³ÌÐò£¬·µ»Øµ±Ç°ÊÂÎñµÄ¼¶±ð
		 */
		long commit_trans() { return conn_ado->CommitTrans(); }
		/**
		 * ÊÂÎñ¹ÜÀí£º³·ÏûÊÂÎñ
		 * @return ¶ÔÓÚÖ§³Ö¶à¼¶ÊÂÎñµÄÇý¶¯³ÌÐò£¬·µ»Øµ±Ç°ÊÂÎñµÄ¼¶±ð
		 */
		long rollback_trans() { return conn_ado->RollbackTrans(); }

		long close() { return conn_ado->Close(); }


		IDispatch* GetDispatch() { return conn_ado; }

		/**
		* »ñÈ¡ADOµÄError½Ó¿Ú
		* ¸Ã½Ó¿Ú¿ÉÒÔ»ñÈ¡sql´íÎóÂë£¬½â¾ö_com_errorÀà²»ÄÜµÃµ½ADO SQLStateµÄÎÊÌâ
		*/
		ADODB::ErrorsPtr GetErrors() { return conn_ado->GetErrors(); }
	};

	/**
	 * Ö´ÐÐÊý¾Ý¿âSQLµÄÓï¾ä
	 */
	class DBTOOL_API PreparedStatement {
		Connection conn; // Êý¾Ý¿âÁ¬½Ó
		ADODB::_CommandPtr stmt; // ×¼±¸ºÃµÄSQLÓï¾ä
		ADODB::ParametersPtr para; // Óï¾äÐèÒªÉèÖÃµÄ²ÎÊý
		// (oledb)Provider²»Ö§³Ö²ÎÊýÐÅÏ¢µ¼³öÊ±£¬±ØÐëÊÖ¶¯´´½¨²¢ÉèÖÃ²ÎÊý
		std::map<long, ADODB::_ParameterPtr> man_para; // ÊÖ¶¯ÉèÖÃµÄ²ÎÊý¼¯
		bool params_refresh; // ÊÇ·ñÖ§³Ö²ÎÊýÐÅÏ¢µ¼³ö£¿
		void set_param_value(long index, _variant_t value, size_t data_len);
	public:
		/**
		 * ¹¹Ôìº¯Êý£º´´½¨Óï¾ä¶ÔÏó
		 * @param conn Êý¾Ý¿âÁ¬½Ó¶ÔÏó¡£×¢ÒâÎÒÃÇÊ¹ÓÃµÄÊÇÒ»¸ö¶ÔÏó£¬¶ø²»ÊÇÒ»¸öÖ¸Õë(ÎªÁËÓëÇ°ÆÚ´úÂë¼æÈÝ)¡£
		 */
		PreparedStatement(Connection conn);
		/**
		 * Îö¹¹º¯Êý£ºÇåÀí¹¤×÷
		 */
		~PreparedStatement();
		/**
		 * Ö±½ÓÖ´ÐÐ(Ã»ÓÐPrepare)Ò»¸öSQLÓï¾ä¡£
		 * ¶ÔÓÚÃ»ÓÐ²ÎÊýÐèÒªÉèÖÃ£¬Ò»´ÎÐÔÖ´ÐÐµÄÓï¾ä£¬Ê¹ÓÃÕâ¸ö·½·¨¡£
		 * @param sqlString ÒªÖ´ÐÐµÄÓï¾ä¡£
		 * @return Èç¹ûÊÇ²éÑ¯(SELECT)Óï¾ä£¬·µ»ØÒ»¸ö½á¹û¼¯£»
		 * Èç¹ûÊÇÊý¾Ý¸üÐÂ(INSERT/UPDATE)Óï¾ä£¬²»·µ»Ø½á¹û¼¯£¬Çë¶ªÆú·µ»ØµÄ¶ÔÏó¡£
		 */
		ResultSet execute(const char *sqlString);
		/**
		 * ×¼±¸(Prepare)Ò»¸öSQLÓï¾ä¡£
		 * ¶ÔÓÚÐèÒªÉèÖÃ²ÎÊý(¶¯Ì¬SQL)£¬»òÕßÐèÒª·´¸´Ö´ÐÐµÄÓï¾ä£¬¿ÉÒÔÏÈµ÷ÓÃPrepare(const char*)£¬
		 * È»ºó(·´¸´)µ÷ÓÃset<Type>(long,<type>)ÓëExecute()Ö´ÐÐSQLÓï¾ä¡£
		 * @param sqlString Òª×¼±¸µÄÓï¾ä¡£
		 * @return Èç¹ûÊÇ²éÑ¯(SELECT)Óï¾ä£¬·µ»ØÒ»¸ö½á¹û¼¯£»
		 * Èç¹ûÊÇÊý¾Ý¸üÐÂ(INSERT/UPDATE)Óï¾ä£¬²»·µ»Ø½á¹û¼¯£¬Çë¶ªÆú·µ»ØµÄ¶ÔÏó¡£
		 */
		void prepare(const char *sqlString);
		/**
		 * ÉèÖÃ²Ù×÷³¬Ê±Ê±¼ä,Èô²»ÉèÖÃ£¬ADOÄ¬ÈÏµÄÊÇ30s
		 * \param timeout ³¬Ê±Öµ£¬µ¥Î»ÊÇs£¬0±íÊ¾ÎÞÏÞ³¤£¨Ö±µ½²Ù×÷½áÊø»ò³ö´í£©
		 */
		void set_timeout( long timeout );
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸öshortÀàÐÍµÄÖµ
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void set_short(long index, short value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸ölongÀàÐÍµÄÖµ
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void set_long(long index, long value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸öbigIntÀàÐÍµÄÖµ
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void set_bigInt(long index, __int64 value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸öfloatÀàÐÍµÄ¸¡µãÊý
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void set_float(long index, float value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸ödoubleÀàÐÍµÄ¸¡µãÊý
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void set_double(long index, double value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸öboolÀàÐÍµÄÂß¼­Öµ
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void set_bool(long index, bool value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸ö×Ö·û´®
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ(C++ÀàÐÍµÄ×Ö·û´®)
		 */
		void set_string(long index, string value) { set_string(index,value.c_str()); }
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸ö×Ö·û´®
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ(CÀàÐÍµÄ×Ö·û´®)
		 */
		void set_string(long index, const char *value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸ö×Ö·û´®
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ(COMÀàÐÍµÄ×Ö·û´®)
		 */
		void set_string(long index, const _bstr_t &value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸öÊ®½øÖÆÊý(DECIMAL)
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void setDecimal(long index, const DECIMAL &value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾äµÄ²ÎÊýÎªÒ»¸öCOMÀàÐÍ
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param value ²ÎÊýµÄÈ¡Öµ
		 */
		void setVariant(long index, const _variant_t &value);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾ä²ÎÊýÖµÎªÒ»¸ö¶þ½øÖÆÊý¾Ý(×Ö½Ú´®)
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param data Êý¾Ý×Ö½Ú´®µÄÎ»ÖÃ
		 * @param data_len Êý¾ÝµÄ³¤¶È(×Ö½ÚÊý)
		 */
		void set_bytes(long index, const char *data, size_t data_len);
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾ä²ÎÊýÖµÎªÒ»¸ö¶þ½øÖÆÊý¾Ý(×Ö½Ú´®)
		 * ÕâÊÇÒ»¸öC++µ÷ÓÃ£¬·½±ãÉèÖÃ¹Ì¶¨³¤¶ÈµÄÊý¾Ý¡£
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param data ×Ö·ûÊý×é
		 * @tparam data_len Êý¾ÝµÄ³¤¶È(×Ö½ÚÊý)
		 */
		template<size_t data_len>
		void set_bytes(long index, const unsigned char (&data)[data_len]) {
			set_bytes(index, reinterpret_cast<const char *>(data), data_len);
		}
		/**
		 * ÉèÖÃÒÑ×¼±¸ºÃµÄ¶¯Ì¬SQLÓï¾ä²ÎÊýµÄÖµÎª¿Õ(NULL)Öµ
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 */
		void set_null(long index);
		/**
		 * Ö´ÐÐÒ»¸öÒÑ×¼±¸ºÃ(²¢ÇÒÉèÖÃÁËºÏÊÊµÄ²ÎÊý)µÄSQLÓï¾ä¡£
		 * ¶ÔÓÚÐèÒªÉèÖÃ²ÎÊý(¶¯Ì¬SQL)£¬»òÕßÐèÒª·´¸´Ö´ÐÐµÄÓï¾ä£¬¿ÉÒÔÏÈµ÷ÓÃPrepare(const char*)£¬
		 * È»ºó(·´¸´)µ÷ÓÃset<Type>(long,<type>)ÓëExecute()Ö´ÐÐSQLÓï¾ä¡£
		 * ¶ÔÓÚ¶¯Ì¬SQLÓï¾ä£¬ÔÚµ÷ÓÃExecute()Ö®Ç°£¬±ØÐëÃ¿¸ö²ÎÊý¶¼±»ÉèÖÃ¡£
		 * @return Èç¹ûÊÇ²éÑ¯(SELECT)Óï¾ä£¬·µ»ØÒ»¸ö½á¹û¼¯£»
		 * Èç¹ûÊÇÊý¾Ý¸üÐÂ(INSERT/UPDATE)Óï¾ä£¬²»·µ»Ø½á¹û¼¯£¬Çë¶ªÆú·µ»ØµÄ¶ÔÏó¡£
		 */
		ResultSet execute();
		/**
		* Ö´ÐÐÒ»¸öÒÑ×¼±¸ºÃ(²¢ÇÒÉèÖÃÁËºÏÊÊµÄ²ÎÊý)µÄSQLÓï¾ä¡£
		* ²»·µ»Ø½á¹û¼¯
		*/
		void execute_noRecords();

		ResultSet executeAsync();

		void cancel();

	};

	/**
	 * SQL²éÑ¯Óï¾ä·µ»ØµÄ½á¹û¼¯¡£
	 * ×¢ÒâÎÒÃÇ½ÐResultSet¶ø²»ÊÇ(ADOµÄ)RecordSet£¬ÒòÎªÎÒÃÇÖ»ÓÃËüÀ´È¡²éÑ¯µÄ½á¹û£¬¶ø²»ÄÜÓÃËüÀ´ÐÞ¸ÄÊý¾Ý¿âÄÚÈÝ¡£
	 */
	class DBTOOL_API ResultSet {
		friend ResultSet PreparedStatement::execute();
		friend ResultSet PreparedStatement::execute(const char *);
		bool lastFieldGetWasNull; // ×îºóËùÈ¡ÁÐ(×Ö¶Î)ÖµÊÇ·ñÎªNULL
		ADODB::_RecordsetPtr rs; // Ëù°ü×°µÄADO¼ÇÂ¼¼¯
		ADODB::FieldsPtr flds; // ½á¹û¼¯µÄÁÐ(×Ö¶Î)
		/**
		 * ¹¹Ôìº¯Êý£º´´½¨¶ÔÏó¡£
		 * ±»¶¨ÒåÎªprivate£¬ÊÇÎªÁË½ûÖ¹ÓÃ»§×Ô¼º´´½¨¡£½á¹û¼¯¶ÔÏóÖ»ÄÜÓÉPreparedStatement¶ÔÏó·µ»Ø¡£
		 */
		ResultSet(ADODB::_RecordsetPtr r) : rs(r), flds(r->Fields), lastFieldGetWasNull(true) {}
	public:
		/**
		 * Îö¹¹º¯Êý£º×ÊÔ´ÇåÀí¹¤×÷¡£
		 */
		~ResultSet() { rs = NULL; flds = NULL; }
		/**
		 * ÊÇ·ñÒÑµ½Êý¾Ý¼¯ºÏµÄÄ©Î²¡£
		 * @return Èç¹û·µ»Øtrue£¬±íÊ¾ÒÑµ½½á¹û¼¯Ä©Î²£¬Ã»ÓÐÓÐÐ§µÄÊý¾ÝÐÐ¿ÉÓÃ£»
		 * Èç¹û·µ»Øfalse£¬ÔòÉÏÒ»¸ö²Ù×÷(MoveNext()»òÕß´ò¿ª½á¹û¼¯²Ù×÷)È¡µÃÁËÓÐÐ§µÄÊý¾ÝÐÐ¡£
		 */
		bool db_eof() { return rs->AdoEOF != 0; }
		/**
		 * ½á¹ûÊÇ·ñÒÑµ½Êý¾Ý¼¯ºÏ¶¥¶Ë¡£
		 * @return Èç¹û·µ»Øtrue£¬±íÊ¾ÒÑµ½½á¹û¼¯¶¥¶Ë£¬ÉÏÒ»¸öMovePrev()Ã»ÓÐÈ¡µÃÓÐÐ§µÄÊý¾ÝÐÐ£»
		 * Èç¹û·µ»Øfalse£¬±êÊ¶ÉÏÒ»¸öMovePrev()²Ù×÷È¡µÃÁËÓÐÐ§µÄÊý¾ÝÐÐ
		 */
		bool db_bof() { return rs->BOF != 0; }
		/**
		 * ¹Ø±Õ½á¹û¼¯¡£
		 * ´Ó×ÊÔ´¹ÜÀíµÄ½Ç¶È£¬Õâ¸ö·½·¨µÄµ÷ÓÃºÜÖØÒª¡£Èç¹ûÓÃ»§ÒÑ¾­²»ÔÙÊ¹ÓÃÒ»¸ö½á¹û¼¯£¬Ó¦¸Ã¹Ø±ÕËü¡£
		 */
		void close() {
			if ( rs->GetState() == ADODB::adStateOpen )
				rs->Close();
		}
		/**
		 * ½á¹û¼¯ä¯ÀÀ£ºÕýÐò(´ÓÇ°ÍùºóÒÆ¶¯)·ÃÎÊÏÂÒ»ÐÐÊý¾Ý¡£
		 * move_next()Ö®ºó£¬Ó¦¸Ãµ÷ÓÃDBEOF()À´ÅÐ¶ÁÊÇ·ñÈ¡µÃÓÐÐ§µÄÊý¾Ý¡£
		 */
		void move_next() { rs->MoveNext(); }
		/**
		 * ½á¹û¼¯ä¯ÀÀ£ºÄæÐò(´ÓºóÍùÇ°ÒÆ¶¯)·ÃÎÊÏÂÒ»ÐÐÊý¾Ý¡£
		 * MovePrev()Ö®ºó£¬Ó¦¸Ãµ÷ÓÃBOF()À´ÅÐ¶ÁÊÇ·ñÈ¡µÃÓÐÐ§µÄÊý¾Ý¡£
		 */
		void move_prev() { rs->MovePrevious(); }
		/**
		 * ½á¹û¼¯ä¯ÀÀ£ºÄæÐò(´ÓºóÍùÇ°ÒÆ¶¯)·ÃÎÊµÚÒ»ÐÐÊý¾Ý¡£
		 * MoveFirst()Ö®ºó£¬Ó¦¸Ãµ÷ÓÃBOF()À´ÅÐ¶ÁÊÇ·ñÈ¡µÃÓÐÐ§µÄÊý¾Ý¡£
		 */
		void move_first() { rs->MoveFirst(); }
		/**
		 * ½á¹û¼¯ä¯ÀÀ£ºÕýÐò(´ÓÇ°ÍùºóÒÆ¶¯)·ÃÎÊ×îºóÒ»ÐÐÊý¾Ý¡£
		 * MoveLastt()Ö®ºó£¬Ó¦¸Ãµ÷ÓÃDBEOF()À´ÅÐ¶ÁÊÇ·ñÈ¡µÃÓÐÐ§µÄÊý¾Ý¡£
		 */
		void move_last() { rs->MoveLast(); }
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªshortÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 */
		short get_short(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªlongÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		long get_long(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªbigintÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		__int64 get_bigInt(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªfloatÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		float get_float(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªdoubleÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		double get_double(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªboolÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		bool get_bool(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªDECIMALÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		DECIMAL getDecimal(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢°ÑÈ¡µÃµÄÖµ×ª»»ÎªCOM×Ö·û´®ÀàÐÍ¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ£¬¶ÔÓÚ¿É¿ÕµÄÁÐ£¬ÐèÒª½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		string get_string(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬²¢ÇÒÓÃCOMµÄÍ¨ÓÃÀàÐÍ·µ»Ø¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @return È¡µÃµÄÁÐÖµ¡£
		 * ¶ÔÓÚ¿É¿ÕµÄÁÐ£¬¿ÉÒÔ½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		_variant_t getVariant(long index);
		/**
		 * ´Óµ±Ç°ÐÐ»ñÈ¡Êý¾Ý×Ö¶Î(ÁÐÖµ)£¬ÕâÊÇ°ÑÁÐÖµµ±³ÉÒ»¸ö¶þ½øÖÆÊý¾Ý(×Ö½Ú´®)À´»ñÈ¡¡£
		 * ×¢Òâ£¬Õâ²»ÊÇ×ª»»£¬Èç¹ûÊý¾Ý¿âÀï¶¨ÒåµÄÁÐ²»ÊÇ¶þ½øÖÆÊý¾Ý£¬»áÒý·¢Òì³£¡£
		 * @param index Òª·ÃÎÊµÄÁÐÔÚ½á¹û¼¯ÖÐÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý¡£
		 * @param buffer ´æ·Å½á¹û¡£ÆäÄÚ´æÓÉgetBytes()·ÖÅä£¬ÓÉµ÷ÓÃÕß¸ºÔðÊÍ·Å(ÓÃdelete[])¡£
		 * @return È¡µÃµÄ¶þ½øÖÆÊý¾ÝµÄ×Ö½ÚÊý¡£
		 * ¶ÔÓÚ¿É¿ÕµÄÁÐ£¬¿ÉÒÔ½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		size_t get_bytes(long index, char *&buffer);
		/**
		 * »ñÈ¡Ö¸¶¨³¤¶ÈµÄ¶þ½øÖÆÊý¾Ý(×Ö½Ú´®)
		 * ÕâÊÇÒ»¸öC++µ÷ÓÃ£¬·½±ãÓÃ¹Ì¶¨¹Ì¶¨³¤¶È»í³öÈ¥»ñµÃÊý¾Ý¡£
		 * @param index ²ÎÊýµÄÎ»ÖÃ£¬´Ó0¿ªÊ¼¼ÆÊý
		 * @param data ×Ö·ûÊý×é
		 * @tparam data_len Êý¾ÝµÄ³¤¶È(×Ö½ÚÊý)
		 */
		template<size_t data_len>
		size_t get_bytes(long index, unsigned char (&data)[data_len]);
		/**
		 * ×îºóÒ»´Îµ÷ÓÃget<Type>(long...)·½·¨·µ»ØµÄÁÐÖµÊÇ·ñÎª¿Õ(NULL)Öµ£¿
		 * ×¢ÒâÊÇwasNull()¶ø²»ÊÇisNull()£¬ËùÒÔÒ»¶¨ÒªÏÈµ÷ÓÃget<Type>(long...)·½·¨¡£
		 * @return true±êÊ¶×îºó·µ»ØÁÐµÄÖµÎªNULL£¬false±êÊ¶×îºó·µ»ØÁÐÖµÓÐÐ§¡£
		 * ¶ÔÓÚ¿É¿ÕµÄÁÐ£¬¿ÉÒÔ½ô¸ú×Åµ÷ÓÃwasNull()À´ÅÐ¶ÏÊÇ·ñÎªNULL¡£
		 */
		bool was_null() { return lastFieldGetWasNull; }
		/**
		 * ½á¹û¼¯µÄÁÐÊý
		 * @return ·µ»Ø½á¹û¼¯ÖÐ×Ö¶Î(ÁÐ)µÄÊýÄ¿¡£
		 */
		long col_count() { return flds->Count; }
	};
}

//================================================================
// ÒÔÏÂÎªÊµÏÖ¡£²»±ØÔÙÍùÏÂ¿´ÁË;-)
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
	//Ê¹ÓÃ¿Í»§¶ËÓÎ±ê
	//Ä¬ÈÏÇé¿öÊÇ·þÎñÆ÷¶ËÓÎ±ê£ºadUseServer£¬¸ÃÓÎ±ê²»Ö§³ÖMovePreviousµÈ²Ù×÷¡£
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
	// VC < 7²»ÄÜÖ±½Ó´¦Àí__int64:(½èÓÃDECIMAL
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
	// VC < 7²»ÄÜÖ±½Ó´¦Àí__int64:(½èÓÃDECIMAL
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

/*ÉùÃ÷£º¸Ã²ÄÁÏ(°üÀ¨µ«²»ÏÞÓÚ±êÌâ¡¢ÕýÎÄ¡¢¸½¼þ¡¢¸½¼þÄÚÐÅÏ¢µÈ)¹é±±¾©¹ú¿Æ»·Óî¿Õ¼ä¼¼ÊõÓÐÏÞ¹«Ë¾ËùÓÐ£¬²¢ÇÒ½ö¹©±±¾©¹ú¿Æ»·Óî¿Õ¼ä¼¼ÊõÓÐÏÞ¹«Ë¾ÓÃÓÚÌØ¶¨ÓÃÍ¾¡£ÉÏÊö×ÊÁÏÐèÒª¾ø¶Ô±£ÃÜ¡£
*ÈÎºÎ½Ó´¥¡¢ÔËËÍ»òÕßÊ¹ÓÃ¸Ã×ÊÁÏµÄÈËÔ±¡¢²¿ÃÅ»òÕßµ¥Î»¶¼±ØÐë±£ÃÜºÍ×ñÊØÏà¹Ø±£ÃÜ¹æ¶¨£¬Î´¾­¹«Ë¾¡¢Àûº¦¹ØÏµÈËµÄÊéÃæÐí¿É²»µÃÐ¹Â¶»òÕßÓÃÓÚÈÎºÎÆäËûÓÃÍ¾¡£
*ÈÎºÎ¸öÈË¡¢²¿ÃÅ»òÕßµ¥Î»²»ÄÜ¸´ÖÆ¡¢ÖØÖÆÉÏÊö×ÊÁÏ£¬Ò²²»ÄÜ½øÐÐ·´±àÒë¡¢·´Ïò¹¤³Ì¡¢·Ö²ð¡¢ÐÞ¸Ä»òÑÜÉúÈÎºÎ×ÊÁÏ»òÕß²¿·Ö×ÊÁÏ¡£
*ÑÏ½ûÎ´¾­ÊÚÈ¨Ê¹ÓÃÕâÐ©×ÊÁÏ»òÕß²¿·Ö×ÊÁÏ¡£ 
*These materials including, but not limited to data£¬datum, document, reference, 
*information are owned by Beijing UCAS Space Technology Co.,Ltd, 
*and can only be used for special purpose by Beijing UCAS Space Technology Co.,Ltd. 
*Those materials mentioned above are strictly confidential. 
*Any person, department or entity shall keep them secret and observe the secrecy provisions when contacting,delivering or using these materials. 
*Any of these materials shall not be disclosed or used in other ways without the written permission of Beijing UCAS Space Technology Co.,Ltd and the interested party submitting it. 
*Any person, department or entity may not copy or reproduce those materials mentioned above nor may decompile, reverse engineer, disassemble, modify or create derivative works of any of these materials, or any portion thereof. 
*Any unauthorized use of the materials or any portion thereof is strictly prohibited. */
 
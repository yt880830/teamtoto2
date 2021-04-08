package dao;

import java.sql.Connection;
import java.sql.PreparedStatement;

public class TourokuDAO extends DAO {
	public boolean insert(String name, String username,String password,String address,String mailaddress) throws Exception {
		Connection con=getConnection();
		con.setAutoCommit(false);
			PreparedStatement st=con.prepareStatement(
					"insert into account (name,username,password,address,mailaddress) values(?,?,?,?,?)");    //ここに追加
			st.setString(1, name);
			st.setString(2, username);
			st.setString(3, password);
			st.setString(4, address);
			st.setString(5, mailaddress);
//			st.setString(6, icon);

			int line=st.executeUpdate();
			st.close();

			if (line!=1) {
				con.rollback();
				con.setAutoCommit(true);
				con.close();
				return false;
			}

			con.commit();
			con.setAutoCommit(true);
			con.close();
				return true;
			}
	}


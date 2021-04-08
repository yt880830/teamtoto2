package dao;

import java.sql.Connection;
import java.sql.PreparedStatement;

public class ItemtourokuDAO extends DAO {
	public boolean insert(int id,String username,int price, String title,String introduction,String genre) throws Exception {
		Connection con=getConnection();
		con.setAutoCommit(false);
			PreparedStatement st=con.prepareStatement(
					"insert into item (id,username,price,title,introduction,genre) values(?,?,?,?,?,?)");    //ここに追加

			st.setInt(1, id);
			st.setString(2, username);
			st.setInt(3, price);
			st.setString(4, title);
			st.setString(5, introduction);
			st.setString(6, genre);

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

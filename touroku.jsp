<%@page contentType="text/html; charset=UTF-8" %>
<%@include file="../header.html" %>
<%@include file="../grasys/menu.jsp" %>


<form  action="/GRASYS/touroku/Touroku.action" method="post" >
<p>会員情報の入力をお願いします。</p>
<p>名前<input type="text" name="name"  placeholder="田中太郎"></p>
<p>登録アカウント名<input type="text" name="username"　 placeholder="tanatti"></p>
<p>登録パスワード<input type="password" name="password"></p>
<p>住所<textarea name="address" cols="30" rows="5"></textarea></p>     <%-- 仮　後で理解する --%>
<p>メールアドレス<input type="email" name="mailaddress"></p>

<p><input type="submit" value="次へ"></p>
</form>

<%@include file="../footer.html" %>

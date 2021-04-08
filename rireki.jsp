<%@page contentType="text/html; charset=UTF-8" %>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@include file="../header.html" %>
<%@include file="menu.jsp" %>

<p>検索キーワードを入力してください。</p>
<form action="/book/chapter25/Rireki.action" method="post">
<input type="text" name="keyword">
<input type="submit" value="検索">
</form>
<hr>

<table style="border-collapse:separate;border-spacing:10px;">
<c:forEach var="purchase , customer" items="${list}">
	<tr>
	<td>商品${purchase.id}</td>
	<td>${product_id}</td>
	<td>${purchase.product_name}</td>
	<td>${purchase.product_count}</td>
	<td>${customer.id}</td>
	<td>${purchase.product_price}円</td>
	<td>${customer.login}</td>
	<td><a href="CartAdd.action?id=${purchase.product_id}">カートに追加</a></td>
	</tr>
</c:forEach>
</table>

<%@include file="../footer.html" %>

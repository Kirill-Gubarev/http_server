document.addEventListener("DOMContentLoaded", function () {
    const user = JSON.parse(localStorage.getItem("user"));

    if (user) {
        // Заполняем информацию о пользователе
        document.getElementById("fullname").textContent = user.fullname;
        document.getElementById("email").textContent = user.email;
        document.getElementById("login").textContent = user.login;
        document.getElementById("balance").textContent = `${user.balance} руб`;

        document.getElementById("logout-btn").addEventListener("click", function (event) {
            event.preventDefault();
            localStorage.removeItem("user");
            window.location.href = "/"; // Перенаправляем на страницу входа
        });
    } else {
        // Если пользователь не авторизован, редирект на страницу входа
        window.location.href = "login";
    }
});

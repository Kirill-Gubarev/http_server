async function fetchProducts() {
    try {
        const baseUrl = window.location.hostname === 'localhost' ? 'http://localhost' : 'http://wildmonkey.ru';
        const response = await fetch(`${baseUrl}/fruits`);
        const products = await response.json();

        localStorage.setItem('products', JSON.stringify(products));

        const productList = document.getElementById('product-list');
        productList.innerHTML = ''; // Очистить список перед добавлением новых товаров

        // Создание и добавление карточек товаров в список
        products.forEach(product => {
            const productElement = document.createElement('div');
            productElement.classList.add('product');

            const productHTML = `
                <img src="${product.image}" alt="${product.name}">
                <p>${product.name} - ${product.price} руб/кг</p>
                <button>В корзину</button>
            `;
            productElement.innerHTML = productHTML;
            productList.appendChild(productElement);
        });
    } catch (error) {
        console.error('Ошибка при загрузке товаров:', error);
    }
}

document.addEventListener('DOMContentLoaded', fetchProducts);
document.addEventListener("DOMContentLoaded", function () {
    const userMenu = document.getElementById("user-menu");

    // Проверяем наличие пользователя в localStorage
    const user = JSON.parse(localStorage.getItem("user"));

    if (user) {
        // Обновляем меню для авторизованного пользователя
        userMenu.innerHTML = `
            <a href="#" style="margin-right: 10px;"><i class="fas fa-shopping-cart"></i> Корзина</a>
            <span id="balance">
                <i class="fas fa-wallet"></i> Баланс: ${user.balance} руб
            </span>
            <a href="profile.html"><i class="fas fa-user"></i> ${user.login}</a>
        `;
    }
});

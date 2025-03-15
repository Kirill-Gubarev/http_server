async function fetchProducts() {
    try {
        const baseUrl = window.location.hostname === 'localhost' ? 'http://localhost' : 'http://wildmonkey.ru';
        const response = await fetch(`${baseUrl}/fruits`);
        const products = await response.json();

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

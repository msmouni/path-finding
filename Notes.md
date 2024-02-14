if ('serviceWorker' in navigator) {
    window.addEventListener('load', () => {
        navigator.serviceWorker.register('/service-worker.js', { scope: '/' })
            .then((registration) => {
                console.log('Service worker registered with scope:', registration.scope);
            })
            .catch((error) => {
                console.error('Service worker registration failed:', error);
            });
    });
}


# If the service worker is register like this: navigator.serviceWorker.register('/service-worker.js')

And the service worker script is located at the root of the web application (/service-worker.js), then the default scope will be '/'. This means that the service worker will control all pages within the same origin as the service worker script.

However, if the service worker script is located in a subdirectory, such as /sw/service-worker.js, the default scope will be './sw/'. This means that the service worker will only control pages within the same origin and path as the service worker script.
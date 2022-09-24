# Classic Cornhole but With More Robots

## Client app development build instructions
1. Download the latest version of [Node.js](https://nodejs.org/en/) following the installation instructions
1. Fetch the dependencies
   1. In powershell or a terminal, navigate to the client app directory `tele_cornhole/client-app`
   1. Run `npm i`, this may take a few minutes to complete
1. In the `client-app` directory, run `npm start` to serve the development server on `localhost:3000`
## Client app production build instructions
1. Follow steps (1) and (2) above
1. In the `client-app` directory, run `npm run build`
1. Globally install serve by running `npm i -g serve`
1. Once the build completes, run `npm serve -s build -l 3000` from the `client-app` directory to serve the production build on `localhost:3000`
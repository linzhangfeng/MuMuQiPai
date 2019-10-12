import Vue from 'vue'
import Router from 'vue-router'
import HelloWorld from '@/components/HelloWorld'
import Login from '@/components/Login'
import Home from '@/components/Home'
import Page1 from '@/components/Page1'
import Page2 from '@/components/Page2'
import Page3 from '@/components/Page3'
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'login',
      component: Login
    },
    {
      path: '/Login',
      name: 'login',
      component: Login
    },
    {
      path: '/Home',
      name: 'home',
      component: Home,
      children: [
        {
          path: '/Page1',
          component: Page1
        },
        {
          path: '/Page2',
          component: Page2
        },
        {
          path: '/Page3',
          component: Page3
        }
      ]
    }
  ],
})
const originalPush = Router.prototype.push;
Router.prototype.push = function push(location) {
  return originalPush.call(this, location).catch(err => err);
};
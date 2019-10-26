import Vue from 'vue'
import Router from 'vue-router'
import HelloWorld from '@/components/HelloWorld'
import Login from '@/components/Login'
import Home from '@/components/Home'
import UserInfo from '@/components/OnLineOperation/UserInfo'
import GameGategory from '@/components/ProductManager/GameGategory'
import DicType from '@/components/HelpDocument/DicType'
import Dictionary from '@/components/HelpDocument/Dictionary'
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
          path: '/UserInfo',
          component: UserInfo
        },
        {
          path: '/GameGategory',
          component: GameGategory
        },
        {
          path: '/DicType',
          component: DicType
        },
        {
          path: '/Dictionary',
          component: Dictionary
        }
      ]
    }
  ],
})
const originalPush = Router.prototype.push;
Router.prototype.push = function push(location) {
  return originalPush.call(this, location).catch(err => err);
};
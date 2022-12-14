#ifndef MAP_HPP
#define MAP_HPP

#include <iomanip>
#include <cstddef>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <string>
#include <exception>
#include <algorithm>     
#include<iterator>

#include <utility>
 
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stack;
using std::map;
using std::pair;
 
#include "../map/avl.hpp"
#include "iterator.hpp"
#include "../utils/iterator_traits.hpp"
#include "../utils/reverse_iterator.hpp"
 

namespace ft
{
    template< typename Key, typename T, typename Compare = std::less<Key> , typename Allocator = std::allocator<ft::pair<const Key, T> > >
 
    class map
    {
        public:

            ////////////////////
            // map member types //   
            ///////////////////

            typedef Key                                                                                                         key_type;
            typedef T										                                                                    mapped_type;
            typedef ft::pair<const Key, T>                                                                                      value_type;
            typedef Compare                                                                                                     key_compare;
            typedef Allocator                                                                                                   allocator_type;
            typedef typename allocator_type::pointer                                                                            pointer;
            typedef typename allocator_type::const_pointer                                                                      const_pointer;
            typedef typename allocator_type::reference                                                                          reference;
            typedef typename allocator_type::const_reference                                                                    const_reference;
            typedef size_t                                                                                                      size_type;
            typedef ptrdiff_t                                   difference_type;
        
            typedef typename ft::map_iterator<key_type,value_type,key_compare,size_type,allocator_type>                         iterator;
            typedef typename ft::const_map_iterator<key_type,value_type,key_compare,size_type,allocator_type>                   const_iterator;
            typedef typename ft::reverse_iterator<iterator>                                                                     reverse_iterator;
            typedef typename ft::reverse_iterator<const_iterator>                                                               const_reverse_iterator;

            
            class value_compare : public std::binary_function<value_type, value_type, bool>
            {
                private:
                    key_compare  comp;
                    value_compare (key_compare c) : comp(c) {}  // constructed with tree's comparison object
                public:
                    bool operator() (const value_type& x, const value_type& y) const
                    {
                        return comp(x.first, y.first);
                    }
            };

            //////////////////////
            // map: constructors  
            /////////////////////

            // default constructor 
            map (const key_compare& comp = key_compare(),const allocator_type& alloc = allocator_type())
            {
                size_ = 0;
            }
            template <class InputIterator>  //  range constructor
            map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(),const allocator_type& alloc = allocator_type())
            {
                insert(first, last);
            }
            map (const map& x)// copy constructor
            {
                 
                if(x.avl.root)
                {
                        *this->avl.root = *x.avl.root;
                }
                if(x.avl.tmp)
                    *this->avl.tmp = *x.avl.tmp;
                if(x.avl.end_node)
                    *this->avl.end_node = *x.avl.end_node;
                if(x.alloc)
                    this->alloc = x.alloc;
                if(x.key_compare_)
                    this->key_compare_ = x.key_compare_;
                this->size_ = x.size_;
            }
            // operator=
            map& operator= (const map& x) // ?
            {
                //         clear();
                // const_iterator it;
                // for (it = n.begin(); it != n.end(); it++){
                //     insert(*it);
                // }
                if(this != &x)
                {
                    // clear and reassign
                    this->size_ = x.size_;
                    // if(x.avl.root)
                    // {
                    //     this->avl.root = x.avl.root;
                    // }
                    // if(x.avl.tmp)
                    //     this->avl.tmp = x.avl.tmp;
                    // if(x.avl.end_node)
                    //     this->avl.end_node = x.avl.end_node;
                 
                    // this->alloc = x.alloc;
                
                    // this->key_compare_ = x.key_compare_;
                    // this->size_ = x.size_;
                }
                return *this;
            }

            /////////////////
            // map: Capacity
            /////////////////
            size_type size() const
            {
                return size_;
            }
            bool empty() const
            {
                return size() == 0;
            }
            size_type max_size() const
            {
                std::allocator<map<key_compare,T> > A; // max size ig less the std::map because i work with avl
                return A.max_size();
            }

            //////////////////
            // map: Modifiers
            /////////////////
            ft::pair<iterator, bool> insert(const value_type & val)
            {
                typedef ft::pair<iterator, bool> pr;

                if(size_ == 0) 
                {
                    avl.insert(val);        
                    size_++;
                    return pr(begin(),true);
                }
                else
                { 
                    avl.tmp = avl.search(avl.root,val.first);
                    if(avl.tmp)
                        return pr(avl.tmp,false); 
                }
                size_++;
                avl.insert(val);
                iterator it(avl.search(avl.root,val.first));
                return pr(it,true);
            }
            

            template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
            {
                for(; first != last; ++first)
                    insert(*first);
            }

            iterator insert(iterator _where, const value_type& val)
            {
                std::pair<iterator, bool> res = insert(val);
                if (res.second) 
                    return res.first;
                else 
                    return end();
            }
            
            
            void erase (iterator position)
            {
                avl.deleteNode(position.node->data.first);
            }

            size_type erase (const key_type& k)
            {
                if(!avl.search(avl.root,k))
                    return 0;
                avl.deleteNode(k);
                return 1;
            }
            
            void clear()
            {
                avl.inOrder(avl.root);
            }
            
            //////////////////
            // map: Iterators
            /////////////////

            iterator begin()
            {
                avl.tmp = avl.minValue(avl.root);
                if(!avl.tmp)
                    return iterator(avl.end_node);
                return iterator(avl.tmp);
            }
            const_iterator begin() const 
            {
                avl.tmp = avl.minValue(avl.root);
                if(!avl.tmp)
                    return const_iterator(avl.end_node);
                return const_iterator(avl.tmp);
            }
            iterator end()
            {
                return iterator(avl.end_node);
            }
            const_iterator end() const
            {
                return const_iterator(avl.end_node);
            }
            reverse_iterator rbegin()
            {
                return reverse_iterator(avl.getNext(avl.findMX(avl.root)));// this is the last element in avl
            }
            
            const_reverse_iterator rbegin() const
            {
                return const_reverse_iterator(avl.getNext(avl.findMX(avl.root))); // this is the last element in avl
            }
            reverse_iterator rend()// before firt element
            {
                return reverse_iterator(avl.minValue(avl.root));// ? 
            }

            const_reverse_iterator rend() const // before firt element
            {
                return const_reverse_iterator(avl.minValue(avl.root));// ? 
            }

            ////////////////////
            // map: Operations:
            ////////////////////

            iterator find (const key_type& k)
            {
                if(size() == 0)
                    return iterator(avl.end_node);
                avl.tmp = avl.search(avl.root,k);
                if(avl.tmp)
                    return iterator(avl.tmp);
                return iterator(avl.end_node);
            }

            const_iterator find (const key_type& k) const
            {
                if(size() == 0)
                    return const_iterator(avl.end_node);
                avl.tmp = avl.search(avl.root,k);
                if(avl.tmp)
                    return const_iterator(avl.tmp);
                return const_iterator(avl.end_node);
            }

            size_type count (const key_type& k) const
            {
                if(size() == 0)
                    return 0;
                if(avl.search(avl.root,k))
                    return 1;
                return 0;
            }

            // if found the element returns it, if not found returns next greter then k,
            // if k is greter then the max element in map will returns garbage value
            iterator lower_bound (const key_type& k)
            {
                if(size() == 0)
                    return iterator(avl.end_node);
                avl.tmp = avl.search(avl.root,k);
                if(avl.tmp)
                    return iterator(avl.tmp);
                if(key_compare_(k,avl.findMX(avl.root)->data.first))
                    return iterator(avl.findNodeByK(avl.root,k));
                else
                    return iterator(avl.end_node);
            }

            const_iterator lower_bound (const key_type& k) const
            {
                if(size() == 0)
                    return const_iterator(avl.end_node);
                avl.tmp = avl.search(avl.root,k);
                if(avl.tmp)
                    return const_iterator(avl.tmp);
                if(key_compare_(k,avl.findMX(avl.root)->data.first))
                    return const_iterator(avl.findNodeByK(avl.root,k));
                else
                    return const_iterator(avl.end_node);
            }

            iterator upper_bound (const key_type& k)
            {
                if(size() == 0)
                    return iterator(avl.end_node);
                avl.tmp = avl.search(avl.root,k);
                if(avl.tmp)
                    return iterator(avl.getNext(avl.tmp));// if element is exist returns next
                if(key_compare_(k,avl.findMX(avl.root)->data.first))// if smaller then max element return next of this k
                    return iterator(avl.findNodeByK(avl.root,k));
                else
                    return iterator(avl.end_node);
            }
            const_iterator upper_bound (const key_type& k) const
            {
                if(size() == 0)
                    return const_iterator(avl.end_node);
                avl.tmp = avl.search(avl.root,k);
                if(avl.tmp)
                    return const_iterator(avl.getNext(avl.tmp));// if element is exist returns next
                if(key_compare_(k,avl.findMX(avl.root)->data.first))// if smaller then max element return next of this k
                    return const_iterator(avl.findNodeByK(avl.root,k));
                else
                    return const_iterator(avl.end_node);
            }
            /////////////////////////
            // map: Element access:
            /////////////////////////

            mapped_type& operator[] (const key_type& k)
            {
                iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
                if (i == end() || key_compare()(k, i->first))
                    i = (insert(value_type(k, mapped_type()))).first;
                return i->second;
            }

            // mapped_type& at (const key_type& k)
            // {
                
            // }

             
            iterator PostIncrement(iterator & it)
            {
                iterator copy = it;
                ++it;
                return copy;
            }

            Node<value_type,size_type> * returnNext(Node<value_type,size_type> * ptr)
            {
                Node<value_type,size_type> * next = avl.getNext(ptr);
                
                return next;
            }

            void erase (iterator first, iterator last)
            {

                if(first == begin() && last == end())
                {
                    avl.inOrder(avl.root);
                    return;
                }
                
                Node<value_type,size_type> * ptr = first.node;
                Node<value_type,size_type> * ptr1 = first.node;

                
 
                
            }
 
            ~map()
            {
                // clear();
            }
            private:
                typedef avl<key_type,value_type,key_compare,size_type,allocator_type> avl_data_strcut;
                avl_data_strcut avl;
                size_type size_;
                allocator_type alloc;
                key_compare key_compare_;
    };
    

} 










#endif
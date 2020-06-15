#include  <bits/stdc++.h>
using namespace std;

struct node
{
	int value;
	int frequency;
	node *left,*right,*parent;
};

node* make_node(int value,int frequency)
{
	node* temp=new node();
	temp->value=value;temp->frequency=frequency;
}

bool check_leaf(node* x)
{
	if(x->left==NULL && x->right==NULL)
	return 1;
	else
	return 0;
}

node* leaderleaf(node* root,int frequency)
{
	queue<node*> q;
	if(root){q.push(root);}
	while(!q.empty())
	{
		node* temp=q.front();
		if(check_leaf(temp) && temp->frequency==frequency)
		{
			return temp;
		}
		q.pop();
	    if(temp->right){q.push(temp->right);}
		if(temp->left){q.push(temp->left);}
	}
	return NULL;
}

node* leaderinternal(node* root,int frequency)
{
	queue<node*> q;
	if(root){q.push(root);}
	while(!q.empty())
	{
		node* temp=q.front();
		if(!check_leaf(temp) && temp->frequency==frequency)
		{
			return temp;
		}
		q.pop();
		if(temp->right){q.push(temp->right);}
		if(temp->left){q.push(temp->left);}
	}
	return NULL;
}

string eight_bit(int value)
{
	string s="00000000";
	int i=8;
	while(i>0)
	{
		int c=(value & 1);
		s[i-1]='0'+c;
		value=value>>1;
		i--;
	}
	return s;
}

string print_path(node* root,node* x)
{
	string s="";
	while(x!=root)
	{
		node* parent=x->parent;
		if(parent->left==x){s=s+"0";}
		if(parent->right==x){s=s+"1";}
		x=parent;
	}
	reverse(s.begin(),s.end());
	return s;
}

void swapnode(node* x,node* y)
{
	node* parentx=x->parent;
	node* parenty=y->parent;
	int rightx=0,leftx=0,righty=0,lefty=0;
	if(parentx->left==x){leftx=1;}
	if(parentx->right==x){rightx=1;}
	if(parenty->left==y){lefty=1;}
	if(parenty->right==y){righty=1;}
	x->parent=parenty;
	y->parent=parentx;
	if(leftx==1){parentx->left=y;}
	if(rightx==1){parentx->right=y;}
	if(lefty==1){parenty->left=x;}
	if(righty==1){parenty->right=x;}
}

void update_internal(node* root,node* x)
{
    int flag=0;
	if(x==root){x->frequency++;return;};
	node* parent=x->parent;
	node* leader_leaf=leaderleaf(root,x->frequency+1);
	node* leader_internal=leaderinternal(root,x->frequency);
	if(leader_leaf==NULL){swapnode(x,leader_internal);flag=1;}
	else
	{
		if(leader_internal!=x){swapnode(x,leader_leaf);swapnode(leader_internal,leader_leaf);}
		else{swapnode(x,leader_leaf);}
	}
	x->frequency++;
	if (flag==0){update_internal(root,parent);}
	else{update_internal(root,x->parent);}
}

void update_leaf(node* root,node* x)
{
	int freq=x->frequency;
	node* leader_leaf=leaderleaf(root,freq);
	node* leader_internal=leaderinternal(root,freq);
	if(leader_internal==NULL)
	{
		swapnode(x,leader_leaf);
	}
	else
	{
		if(leader_leaf!=x){swapnode(x,leader_internal);swapnode(leader_internal,leader_leaf);}
		else{if(x->parent!=leader_internal){swapnode(x,leader_internal);};}
	}
	x->frequency=x->frequency+1;;
	update_internal(root,x->parent);
}

int main()
{
	string s="";
	string p;
	cin>>p;
	unordered_map<int,node*> m;
	node* root=make_node(256,0);
	m[256]=root;
	vector<int> v;
	for(int i=0;i<p.size();i++)
	{
		char c;c=p[i];
		int a=c-0;
		v.push_back(a);
	}
	for(int i=0;i<p.size();i++)
	{
		if(m.find(v[i])==m.end())
		{
			node* temp=m[256];
			s=s+print_path(root,temp);
			s=s+eight_bit(v[i]);
			node* left=make_node(256,0);
			node* right=make_node(v[i],1);
			temp->value=258;
			left->parent=temp;right->parent=temp;
			temp->left=left;temp->right=right;
			m[256]=left;
			m[v[i]]=right;
			update_internal(root,temp);
		}
		else
		{
			node* temp=m[v[i]];
			s=s+print_path(root,temp);
			update_leaf(root,temp);
		}
	}
	cout<<s;
	cout<<endl<<s.length()<<endl<<float(s.length())/(8*p.size());

	string q;
	unordered_map<int,node*> md;
	node* droot=make_node(256,0);
	md[256]=droot;
	node* temp1=droot;
	int i=0;
	while(i<=s.size())
    {
        if(check_leaf(temp1))
        {
            int value=0;char c;
            if(temp1->value==256)
            {

                int j=7;
                while(j>=0)
                {
                    value=pow(2,j)*(s[i]-'0')+value;
                    j--;i++;
                }
            }
            else
                value=temp1->value;
            c=value;
            q.push_back(c);
         if(md.find(value)==md.end())
		{
			node* temp2=md[256];
			node* left=make_node(256,0);
			node* right=make_node(value,1);
			temp2->value=258;
			left->parent=temp2;right->parent=temp2;
			temp2->left=left;temp2->right=right;
			md[256]=left;
			md[value]=right;
			update_internal(droot,temp2);
		}
		else
		{
			node* temp2=md[value];
			update_leaf(droot,temp2);
		}
		temp1=droot;
        }
        else
        {
            if(s[i]=='1'){temp1=temp1->right;}
            else{temp1=temp1->left;}
            i++;
        }
        }
cout<<endl<<q;
	return 0;
}

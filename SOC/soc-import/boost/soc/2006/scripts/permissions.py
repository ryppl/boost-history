#  Copyright David Abrahams 2006. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

from projects import projects
import os

def system(cmd):
    print cmd
    os.system(cmd)
    
def permission(args):
    system('trac-admin /usr/local/boost/share/tracs/soc permission '+args)


# prevent the world from spamming the wiki
permission('remove anonymous WIKI_CREATE WIKI_MODIFY TICKET_CREATE TICKET_MODIFY')

permission('add dave TRAC_ADMIN')

import crypt
pass_file = open('/usr/local/boost/etc/svn-passwd', 'a+b')
pass_file.seek(0)
passwds = pass_file.readlines()

existing_users = set()
for l in passwds:
    existing_users.add( l[:l.find(':')] )


new_passwds = []
def soc_id(id):
    permission('add %s soc2006' % id)
    if id not in existing_users:
        new_passwds.append(id + ':' + crypt.crypt(id[::-1],'sc'))
        print 'new_passwords=\n',new_passwds
        existing_users.add(id)

for project,student,mentor in projects:
    soc_id(student)
    soc_id(mentor)

# Append the new passwords
pass_file.seek(0,2)
if passwds[-1][-1] != '\n':
    pass_file.write('\n')
    
pass_file.write('\n'.join(new_passwds))

permission(
    'add soc2006 ' + ' '.join([
        'BROWSER_VIEW',
        'CHANGESET_VIEW',
#          'CONFIG_VIEW',
        'FILE_VIEW',
        'LOG_VIEW',
        'MILESTONE_ADMIN',
#          'MILESTONE_CREATE',
#          'MILESTONE_DELETE',
#          'MILESTONE_MODIFY',
#          'MILESTONE_VIEW',
        'REPORT_ADMIN',
#          'REPORT_CREATE',
#          'REPORT_DELETE',
#          'REPORT_MODIFY',
#          'REPORT_SQL_VIEW',
#          'REPORT_VIEW',
        'ROADMAP_VIEW',
        'SEARCH_VIEW',
        'TICKET_ADMIN',
#          'TICKET_APPEND',
#          'TICKET_CHGPROP',
#          'TICKET_CREATE',
#          'TICKET_MODIFY',
#          'TICKET_VIEW',
        'TIMELINE_VIEW',
#        'TRAC_ADMIN',
        'WIKI_ADMIN',
        'WIKI_CREATE',
        'WIKI_DELETE',
        'WIKI_MODIFY',
        'WIKI_VIEW'
        ]))

    
    
